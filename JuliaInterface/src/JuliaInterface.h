#ifndef JULIAINTERFACE_H_
#define JULIAINTERFACE_H_

#include "src/compiled.h" /* GAP headers */
#include <julia.h>

extern jl_value_t *    JULIA_ERROR_IOBuffer;
extern jl_function_t * JULIA_FUNC_take_inplace;
extern jl_function_t * JULIA_FUNC_String_constructor;
extern jl_function_t * JULIA_FUNC_showerror;

#define JULIAINTERFACE_EXCEPTION_HANDLER                                     \
    if (jl_exception_occurred()) {                                           \
        jl_call2(JULIA_FUNC_showerror, JULIA_ERROR_IOBuffer,                 \
                 jl_exception_occurred());                                   \
        jl_value_t * string_object =                                         \
            jl_call1(JULIA_FUNC_take_inplace, JULIA_ERROR_IOBuffer);         \
        string_object =                                                      \
            jl_call1(JULIA_FUNC_String_constructor, string_object);          \
        ErrorMayQuit(jl_string_data(string_object), 0, 0);                   \
    }

#define INITIALIZE_JULIA_CPOINTER(name)                                      \
    {                                                                        \
        jl_value_t * gap_ptr;                                                \
        jl_sym_t *   gap_symbol;                                             \
        gap_ptr = jl_box_voidpointer(name);                                  \
        gap_symbol = jl_symbol("gap_" #name);                                \
        JULIAINTERFACE_EXCEPTION_HANDLER                                     \
        jl_set_const(jl_main_module, gap_symbol, gap_ptr);                   \
        JULIAINTERFACE_EXCEPTION_HANDLER                                     \
    }

// Internal Julia access functions

// SET_JULIA_OBJ(o,v)
//
// Sets the value of the julia object GAP object
// to the julia value pointer v.
void SET_JULIA_OBJ(Obj, jl_value_t *);

// GET_JULIA_OBJ(o)
//
// Returns the julia value pointer
// from the julia object GAP object o.
jl_value_t * GET_JULIA_OBJ(Obj);

// Internal
Obj JuliaFunctionTypeFunc(Obj);

// Internal
Obj JuliaObjectTypeFunc(Obj);

// IS_JULIA_OBJ(o)
//
// Checks if o is a julia object GAP object.
#define IS_JULIA_OBJ(o) (TNUM_OBJ(o) == T_JULIA_OBJ)

// Internal
UInt T_JULIA_OBJ = 0;

// NewJuliaFunc(f,autoConvert)
//
// Creates a new julia function GAP object
// from the julia function pointer f.
Obj NewJuliaFunc(jl_function_t * f, int autoConvert);

// NewJuliaObj(v)
//
// Creates a new julia object GAP object
// from the julia value pointer v.
Obj NewJuliaObj(jl_value_t *);

// get_module_from_string( name )
//
// Returns a julia module pointer to the module <name>.
jl_module_t * get_module_from_string(char * name);

#endif
