cdef extern from "sqlite3.h":
    int SQLITE_UTF8
    int SQLITE_DETERMINISTIC
    int SQLITE_LIMIT_FUNCTION_ARG
    int SQLITE_OK

    ctypedef struct sqlite3:
        pass

    ctypedef struct sqlite3_context:
        pass

    ctypedef struct sqlite3_value:
        pass

    ctypedef void (*scalarfunc)(sqlite3_context*, int, sqlite3_value**)
    ctypedef void (*stepfunc)(sqlite3_context*, int, sqlite3_value**)
    ctypedef void (*finalfunc)(sqlite3_context*)

    int sqlite3_create_function(
        sqlite3 *db,
        const char *function_name,
        int number_of_arguments,
        int text_representation,
        void *application_data,
        scalarfunc scalar_function,
        stepfunc step_function,
        finalfunc final_function
    )

    const char *sqlite3_errmsg(sqlite3 *db)


cdef extern from "pysqlite/connection.h":
    ctypedef class sqlite3.Connection [object pysqlite_Connection]:
        cdef sqlite3 *db


cpdef object register_function_pointer(
    Connection con,
    const char *name,
    int narg,
    Py_ssize_t address,
    bint is_deterministic=True
):
    assert -1 <= narg <= SQLITE_LIMIT_FUNCTION_ARG, \
        'Number of arguments must be between -1 and {:d}'.format(
            SQLITE_LIMIT_FUNCTION_ARG
        )

    cdef int result = sqlite3_create_function(
        con.db,
        name,
        narg,
        SQLITE_UTF8 | (is_deterministic & SQLITE_DETERMINISTIC),
        NULL,
        # for some reason Cython fails when using the `scalarfunc` type
        <void (*)(sqlite3_context*, int, sqlite3_value**)> address,
        NULL,
        NULL,
    )

    if result != SQLITE_OK:
        raise RuntimeError(sqlite3_errmsg(con.db))