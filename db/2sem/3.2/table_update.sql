-- NOTE: query 
UPDATE actors SET rolesname = jsonb_set(rolesname, '{roles, 0, year}', '"2025"') WHERE id = <ID>;
