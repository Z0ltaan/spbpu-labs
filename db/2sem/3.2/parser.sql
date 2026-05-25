CREATE EXTENSION IF NOT EXISTS plpython3u;

CREATE OR REPLACE PROCEDURE import_actors_from_file(file_path TEXT)
AS $$
import re
import json

line_re = re.compile(r'^(?P<name>[^\t]+)\t+(?P<role_data>.+)')
subsequent_role_re = re.compile(r'^\t+(?P<role_data>.+)')

role_parser = re.compile(
    r'^(?P<title>.+?)\s+'
    r'\((?P<year>\d{4}|\?{4})\S*\)'
    r'(?:\s+(?P<tags>(?:\((?:V|TV|VG|archive footage|uncredited|voice)\)\s*)+))?'
    r'(?:\s+\{(?P<series>[^\}]+)\})?'
    r'(?:\s+\(as\s+(?P<alias>[^\)]+)\))?'
    r'(?:\s+\[(?P<character>[^\]]+)\])?'
    r'(?:\s+<(?P<credit>\d+)>)?'
)

def parse_role_line(role_str):
    m = role_parser.search(role_str.strip())
    if not m:
        return None
    
    res = {
        "title": m.group('title').strip('" '),
        "year": m.group('year').replace('????', '0000')
    }
    
    tags = m.group('tags')
    if tags:
        if '(V)' in tags: res['type_video'] = True
        if '(TV)' in tags: res['type_tv'] = True
        if '(VG)' in tags: res['type_game'] = True
        if '(uncredited)' in tags: res['uncredited'] = True
        if '(voice)' in tags: res['voice'] = True
        if '(archive footage)' in tags: res['archive'] = True

    # Дополнительные поля, если они есть
    if m.group('series'):    res['series'] = m.group('series')
    if m.group('alias'):     res['as'] = m.group('alias')
    if m.group('character'): res['character'] = m.group('character')
    if m.group('credit'):    res['credit_pos'] = int(m.group('credit'))
    
    return res

with open(file_path, 'r', encoding='latin-1') as f:
    current_name = None
    roles = []
    
    # Пропуск заголовка
    for line in f:
        if line.startswith('----'): break

    plan = plpy.prepare("INSERT INTO actors (first_name, last_name, roles) VALUES ($1, $2, $3)", ["text", "text", "jsonb"])

    for line in f:
        if not line.strip() or line.startswith('----'): continue
        
        name_match = line_re.match(line)
        if name_match:
            # Сохраняем предыдущего
            if current_name and roles:
                parts = current_name.split(', ', 1)
                last = parts[0] if len(parts) > 0 else ""
                first = parts[1] if len(parts) > 1 else ""
                plpy.execute(plan, [first, last, json.dumps({"roles": roles})])
            
            current_name = name_match.group('name').strip()
            roles = []
            role_info = parse_role_line(name_match.group('role_data'))
            if role_info: roles.append(role_info)
            
        else:
            role_match = subsequent_role_re.match(line)
            if role_match and current_name:
                role_info = parse_role_line(role_match.group('role_data'))
                if role_info: roles.append(role_info)

    # Последний актер
    if current_name and roles:
        parts = current_name.split(', ', 1)
        plpy.execute(plan, [parts[1] if len(parts)>1 else "", parts[0], json.dumps({"roles": roles})])

$$ LANGUAGE plpython3u;

