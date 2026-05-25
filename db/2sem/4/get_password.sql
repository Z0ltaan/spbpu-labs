CREATE FUNCTION get_safe_password_c(varchar) 
RETURNS varchar 
AS 'libtpass', 'get_safe_password_c' 
LANGUAGE C STRICT;

