#include <postgres.h>
#include <fmgr.h>
#include <stdlib.h>
#include <string.h>
#include <utils/builtins.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(get_safe_password_c);

Datum get_safe_password_c(PG_FUNCTION_ARGS) {
  text *input_text = PG_GETARG_TEXT_PP(0);
  char *input = text_to_cstring(input_text);
  int len = strlen(input);

  char *result = palloc(len + 1);
  result[0] = '\0';
  int res_idx = 0;

  int current_pos = 50;

  for (int i = 0; i < len; i += 3) {
    if (i + 2 >= len)
      break;

    char direction = input[i];
    char val_str[3] = {input[i + 1], input[i + 2], '\0'};
    int value = atoi(val_str);

    if (direction == 'R') {
      current_pos = (current_pos + value) % 100;
    } else if (direction == 'L') {
      current_pos = (current_pos - value + 100) % 100;
    }

    if (current_pos == 0) {
      strcpy(&result[res_idx], val_str);
      res_idx += 2;
    }
  }
  result[res_idx] = '\0';

  PG_RETURN_TEXT_P(cstring_to_text(result));
}
