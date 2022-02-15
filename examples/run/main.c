/*
RvnicRaven lisp interpreter

Written in 2022 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//-------------------------------------

//Internal includes
#include "../../src/RvR_lisp.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
static void lisp_init();
static int lisp_ccall(int number, RvR_lisp_object *args);
static RvR_lisp_object *lisp_lcall(int number, RvR_lisp_object *args);
static void lisp_lset(int number, RvR_lisp_object *arg);
static RvR_lisp_object *lisp_lget(int number);
static void lisp_lprint(int number);
//-------------------------------------

//Function implementations

int main(int argc, char **argv)
{
   if(argc<2)
      return 1;

   char code_buff[1024];
   snprintf(code_buff,1024,"(load \"%s\")",argv[1]);
   const char *code = code_buff;

   RvR_lisp_init(lisp_init,lisp_ccall,lisp_lcall,lisp_lset,lisp_lget,lisp_lprint);

   RvR_lisp_object *prog = RvR_lisp_compile(&code);
   RvR_lisp_eval(prog);

   return 0;
}

static void lisp_init()
{

}

static int lisp_ccall(int number, RvR_lisp_object *args)
{
   return 0;
}

static RvR_lisp_object *lisp_lcall(int number, RvR_lisp_object *args)
{
   return NULL;
}

static void lisp_lset(int number, RvR_lisp_object *arg)
{

}

static RvR_lisp_object *lisp_lget(int number)
{
   return NULL;
}

static void lisp_lprint(int number)
{

}
//-------------------------------------
