CREATE EXTENSION IF NOT EXISTS plpython3u;

CREATE OR REPLACE PROCEDURE import_actors_from_file(file_path TEXT)
AS $$
import re
import json

# Регулярки: 
# 1. Для строки с именем актера (Фамилия, Имя)
name_re = re.compile(r'^([^\t]+),\s+([^\t]+)')
# 2. Для строки с ролью (начинается с табуляции)
role_re = re.compile(
    r'^\t+(?P<title>.+?)\s+'          
    r'\((?P<year>\d{4}|\?{4})\S*\)\s*' 
    r'(?P<types>(?:\([^\)]+\)\s*)*)'   
    r'(?:\{(?P<series>[^\}]+)\}\s*)?'  
    r'(?:\((?P<as_char>as [^\)]+)\)\s*)?' 
    r'(?:\[(?P<char>[^\]]+)\]\s*)?'    
    r'(?:<(?P<credit>\d+)>)?'
)

type_map = {
    '(V)': 'typeVI', '(TV)': 'typeTV', '(VG)': 'typeVG', 
    '(archive footage)': 'typeAF', '(uncredited)': 'typeUC', '(voice)': 'typeVO'
}

-- try:
with open(file_path, 'r', encoding='latin-1') as f:
    current_actor = None
    roles = []
    count = 0

    for line in f:
        # Ищем имя актера
        name_match = name_re.match(line)
        if name_match:
            # Если уже был актер, сохраняем его перед началом нового
            if current_actor and roles:
                plan = plpy.prepare("INSERT INTO actors (FirstName, LastName, RolesName) VALUES ($1, $2, $3)", ["text", "text", "jsonb"])
                plpy.execute(plan, [current_actor['first'], current_actor['last'], json.dumps({"roles": roles})])
                count += 1
            
            current_actor = {'last': name_match.group(1), 'first': name_match.group(2)}
            roles = []
            continue

        # Ищем роль для текущего актера
        role_match = role_re.search(line)
        if role_match and current_actor:
            role = {
                "title": role_match.group('title').strip('" '),
                "year": role_match.group('year').replace('????', '0000')
            }
            
            # Маппинг типов (V, TV, VG...)
            tags = role_match.group('types')
            for tag, key in type_map.items():
                if tag in tags: role[key] = "1"
            
            if role_match.group('series'): role['series name'] = role_match.group('series')
            if role_match.group('char'): role['character name'] = role_match.group('char')
            if role_match.group('credit'): role['credit'] = role_match.group('credit')
            
            roles.append(role)

    # Не забываем сохранить последнего актера
    if current_actor and roles:
        plan = plpy.prepare("INSERT INTO actors (FirstName, LastName, RolesName) VALUES ($1, $2, $3)", ["text", "text", "jsonb"])
        plpy.execute(plan, [current_actor['first'], current_actor['last'], json.dumps({"roles": roles})])
        count += 1

    -- return {count}
print(f"Imported actors: {count}")
-- except Exception as e:
--     return f"Ошибка: {str(e)}"
$$ LANGUAGE plpython3u;

