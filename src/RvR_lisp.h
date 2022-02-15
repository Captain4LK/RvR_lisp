/*
RvnicRaven lisp interpreter

Written in 2022 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _RVR_LISP_H_

#define _RVR_LISP_H_

//New:

#define RVR_LISP_TRACE_EXECUTION 1

#define RVR_LISP_STACK_MAX 256

//Old:

//Lisp profiling
#define RVR_LISP_PROFILE 1

//Lisp type checking
#define RVR_LISP_TYPE_CHECK 1

//Max token length
#define RVR_LISP_MAX_TOKEN_LEN 200

typedef struct RvR_lisp_object RvR_lisp_object;

typedef enum
{
   RVR_LISP_BAD_CELL,   //error catching type
   RVR_LISP_CONS_CELL, RVR_LISP_NUMBER, RVR_LISP_SYMBOL,     RVR_LISP_SYS_FUNCTION, RVR_LISP_USER_FUNCTION,
   RVR_LISP_STRING, RVR_LISP_CHARACTER, RVR_LISP_C_FUNCTION, RVR_LISP_C_BOOL,       RVR_LISP_L_FUNCTION, RVR_LISP_POINTER,
   RVR_LISP_OBJECT_VAR, RVR_LISP_1D_ARRAY,
   RVR_LISP_FIXED_POINT, RVR_LISP_COLLECTED_OBJECT
}RvR_lisp_type;

typedef enum
{
   RVR_LISP_PERM_SPACE,
   RVR_LISP_TMP_SPACE,
   RVR_LISP_USER_SPACE,
   RVR_LISP_GC_SPACE,
}RvR_lisp_space;

struct RvR_lisp_object
{
   //LObject
   uint32_t type;

   union
   {
      //LObjectVar
      struct
      {
         int32_t index;
      }var;

      //LList
      struct
      {
         struct RvR_lisp_object *cdr, *car;
      }list;

      //LNumber
      struct
      {
         int32_t num;
      }num;

      //LRedirect
      struct
      {
         struct RvR_lisp_object *ref;
      }ref;

      //LString
      struct
      {
         char *str;
      }str;

      //LSymbol
      struct
      {
         struct RvR_lisp_object *value;
         struct RvR_lisp_object *function;
         struct RvR_lisp_object *name;
         struct RvR_lisp_object *left, *right;

#if RVR_LISP_PROFILE
         float time_taken;
#endif
      }sym;

      //LSysfunction
      struct
      {
         int32_t min_args;
         int32_t max_args;
         int32_t fun_number;
      }sys;

      //LUserFunction
      struct
      {
         struct RvR_lisp_object *arg_list;
         struct RvR_lisp_object *block_list;
      }usr;

      //LArray
      struct
      {
         struct RvR_lisp_object **data;
         size_t len;
      }arr;

      //LChar
      struct
      {
         uint16_t ch;
      }ch;

      //LPointer
      struct
      {
         void *addr;
      }addr;

      //LFixedPoint
      struct
      {
         int32_t x;
      }fix;
   }obj;
};

typedef void (*RvR_lisp_cinit_func) (void);
typedef int (*RvR_lisp_ccall_func) (int number, RvR_lisp_object *arg);
typedef RvR_lisp_object *(*RvR_lisp_lcall_func) (int number, RvR_lisp_object *arg);
typedef RvR_lisp_object *(*RvR_lisp_lget_func) (int number);
typedef void (*RvR_lisp_lset_func) (int number, RvR_lisp_object *arg);
typedef void (*RvR_lisp_lprint_func) (int number);

//Util
uint32_t RvR_lisp_item_type(RvR_lisp_object *x); //Get type of object
void     RvR_lisp_object_print(RvR_lisp_object *o);  //Print a objects contents
void     RvR_lisp_push_onto_list(RvR_lisp_object *object, RvR_lisp_object **list);
void     RvR_lisp_init(RvR_lisp_cinit_func init, RvR_lisp_ccall_func ccall, RvR_lisp_lcall_func lcall, RvR_lisp_lset_func lset, RvR_lisp_lget_func lget, RvR_lisp_lprint_func lprint);
void     RvR_lisp_uninit();

//Errors
void     RvR_lisp_break(const char *format, ...); //Break execution and start debugger
void     RvR_lisp_print_trace_stack(int max_levels);

//Memory managment
void *RvR_lisp_mark_heap(int heap);
void  RvR_lisp_restore_heap(void *val, int heap);
void  RvR_lisp_tmp_space();
void  RvR_lisp_perm_space();
void  RvR_lisp_use_user_space(void *addr, long size);
void  RvR_lisp_clear_tmp();

//Compiling
int RvR_lisp_read_token(const char **s, char *buffer);
int RvR_lisp_end_of_program(const char *s);

//Object creation/modification
RvR_lisp_object *RvR_lisp_array_create(size_t len, RvR_lisp_object *rest);
RvR_lisp_object *RvR_lisp_fixed_point_create(int32_t x);
RvR_lisp_object *RvR_lisp_object_var_create(int index);
RvR_lisp_object *RvR_lisp_pointer_create(void *addr);
RvR_lisp_object *RvR_lisp_char_create(uint16_t ch);
RvR_lisp_object *RvR_lisp_strings_create(const char *string);
RvR_lisp_object *RvR_lisp_stringsl_create(const char *string, int length);
RvR_lisp_object *RvR_lisp_stringl_create(int length);
RvR_lisp_object *RvR_lisp_user_function_create(RvR_lisp_object *arg_list, RvR_lisp_object *block_list);
RvR_lisp_object *RvR_lisp_sys_function_create(int min_args, int max_args, int fun_number);
RvR_lisp_object *RvR_lisp_c_function_create(int min_args, int max_args, int fun_number);
RvR_lisp_object *RvR_lisp_c_bool_create(int min_args, int max_args, int fun_number);
RvR_lisp_object *RvR_lisp_user_lisp_function_create(int min_args, int max_args, int fun_number);
RvR_lisp_object *RvR_lisp_symbol_create(char *name);
RvR_lisp_object *RvR_lisp_number_create(int32_t num);
RvR_lisp_object *RvR_lisp_list_create();
RvR_lisp_object *RvR_lisp_assoc(RvR_lisp_object *item, RvR_lisp_object *list);
void             RvR_lisp_symbol_function_set(RvR_lisp_object *sym, RvR_lisp_object *fun);
RvR_lisp_object *RvR_lisp_add_c_object(RvR_lisp_object *symbol, int index);
RvR_lisp_object *RvR_lisp_add_c_function(const char *name, int min_args, int max_args, int number);
RvR_lisp_object *RvR_lisp_add_c_bool_function(const char *name, int min_args, int max_args, int number);
RvR_lisp_object *RvR_lisp_add_lisp_function(const char *name, int min_args, int max_args, int number);

//"Getters"
RvR_lisp_object *RvR_lisp_cdr(RvR_lisp_object *o);
RvR_lisp_object *RvR_lisp_car(RvR_lisp_object *o);
char            *RvR_lisp_string(RvR_lisp_object *o);
void            *RvR_lisp_pointer(RvR_lisp_object *o);
int32_t          RvR_lisp_fixed_point(RvR_lisp_object *o);
RvR_lisp_object *RvR_lisp_nth(int num, RvR_lisp_object *list);
void            *RvR_lisp_pointer_value(RvR_lisp_object *pointer);
int32_t          RvR_lisp_number_value(RvR_lisp_object *number);
uint16_t         RvR_lisp_character_value(RvR_lisp_object *c);
int32_t          RvR_lisp_fixed_point_value(RvR_lisp_object *c);
RvR_lisp_object *RvR_lisp_array_get(RvR_lisp_object *o, int x);
RvR_lisp_object *RvR_lisp_symbol_find(const char *name);
RvR_lisp_object *RvR_lisp_symbol_find_or_create(const char *name);
size_t           RvR_lisp_list_length(RvR_lisp_object *list);

//"Setters"
void RvR_lisp_symbol_number_set(RvR_lisp_object *sym, int32_t num);
void RvR_lisp_symbol_value_set(RvR_lisp_object *sym, RvR_lisp_object *val);

//Execution
RvR_lisp_object *RvR_lisp_eval(RvR_lisp_object *o);
RvR_lisp_object *RvR_lisp_eval_block(RvR_lisp_object *list);
RvR_lisp_object *RvR_lisp_eval_function(RvR_lisp_object *sym, RvR_lisp_object *arg_list);
RvR_lisp_object *RvR_lisp_eval_user_function(RvR_lisp_object *sym, RvR_lisp_object *arg_list);
RvR_lisp_object *RvR_lisp_eval_sys_function(RvR_lisp_object *sym, RvR_lisp_object *arg_list);

void *RvR_lisp_atom(RvR_lisp_object *i);

RvR_lisp_object *RvR_lisp_eq(RvR_lisp_object *n1, RvR_lisp_object *n2);
RvR_lisp_object *RvR_lisp_equal(RvR_lisp_object *n1, RvR_lisp_object *n2);


RvR_lisp_object *RvR_lisp_compile(const char **s);

#endif
