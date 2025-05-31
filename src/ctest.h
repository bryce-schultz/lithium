//***********************************************
// File: ctest.h
//
// Author: Bryce Schultz
//
// Purpose: This file contains the CTest 
// framework for testing C/C++ code. It provides 
// macros for assertions, assessments, and 
// exception handling. It also provides a simple 
// way to run tests and print the results. The 
// framework is designed to be easy to use and 
// understand.
//***********************************************

#pragma once

// includes
#include <stdio.h>
#include <fenv.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define CTEST_VERSION "v1.1"

// data used by the framework
#ifdef CTEST
// output file used to print test results
FILE* test_out = NULL;

// used to track all test groups
int ALL_GROUPS_PASS = 1;
// used to track all tests in a group
int ALL_TESTS_PASS = 1;
// used to track current test
int CURRENT_TEST_PASS = 1;

// used to track test and group counts
int GROUP_COUNT = 0;
int GROUP_PASS_COUNT = 0;
int GROUP_TEST_COUNT = 0;
int GROUP_TEST_PASS_COUNT = 0;
int TOTAL_TEST_COUNT = 0;
int TOTAL_TEST_PASS_COUNT = 0;

// used to track times if needed
clock_t TEST_START_TIME;
clock_t TEST_END_TIME;
#endif

// test type
typedef void Test;

// utility macros
#define UNUSED(x) (void)(x)

// test result values
#define PASS 1
#define FAIL 0

// exceptions
#define DIVIDE_BY_ZERO_EXCEPTION FE_DIVBYZERO
#define OVERFLOW_EXCEPTION FE_OVERFLOW
#define UNDERFLOW_EXCEPTION FE_UNDERFLOW
#define INEXACT_EXCEPTION FE_INEXACT
#define INVALID_EXCEPTION FE_INVALID

// main test run macro use to run a test
#define TEST(fn) fprintf(test_out, "    running test: %s\n", #fn); CURRENT_TEST_PASS = 1; GROUP_TEST_COUNT++; TOTAL_TEST_COUNT++; fn(); if (CURRENT_TEST_PASS) { ctest_info("    %s ", #fn); ctest_pass("passed.\n\n"); GROUP_TEST_PASS_COUNT++; TOTAL_TEST_PASS_COUNT++; } else { ctest_info("    %s ", #fn); ctest_fail("failed.\n\n"); ALL_TESTS_PASS = 0; ALL_GROUPS_PASS = 0; }

// print a nice start and end message
#define START_TESTS if (!test_out) { test_out = stderr; } ctest_intro("-----------------------------------------\n"); ctest_intro("          CTest Framework " CTEST_VERSION "\n"); ctest_intro("-----------------------------------------\n");
#define END_TESTS if (ALL_GROUPS_PASS) { ctest_pass("-----------------------------------------\n"); ctest_pass("  %d/%d groups passed. %d/%d tests passed.\n",GROUP_PASS_COUNT, GROUP_COUNT, TOTAL_TEST_PASS_COUNT, TOTAL_TEST_COUNT); ctest_pass("-----------------------------------------\n"); } else { ctest_fail("-----------------------------------------\n"); ctest_fail("  %d/%d groups passed. %d/%d tests passed.\n",GROUP_PASS_COUNT, GROUP_COUNT, TOTAL_TEST_PASS_COUNT, TOTAL_TEST_COUNT); ctest_fail("-----------------------------------------\n"); }

// groups
#define START_GROUP(name) GROUP_COUNT++; GROUP_TEST_COUNT = 0; GROUP_TEST_PASS_COUNT = 0; ALL_TESTS_PASS = 1; ctest_group("-----------------------------------------\n%s\n-----------------------------------------\n", #name);
#define END_GROUP if (ALL_TESTS_PASS) { ctest_pass("    %d/%d tests in the ctest_group passed.\n", GROUP_TEST_PASS_COUNT, GROUP_TEST_COUNT); GROUP_PASS_COUNT++; } else { ctest_fail("    %d/%d tests in the ctest_group passed.\n", GROUP_TEST_PASS_COUNT, GROUP_TEST_COUNT); } ctest_group("-----------------------------------------\n\n");

// assertions
#define assertEqual(a, b)               if (a != b)             { ctest_fail("    ctest_fail %s:%d\n    | reason: %d == %d is false\n",     __FILE__, __LINE__, a, b);      CURRENT_TEST_PASS = 0; return; }
#define assertNotEqual(a, b)            if (a == b)             { ctest_fail("    ctest_fail %s:%d\n    | reason: %d != %d is false\n",     __FILE__, __LINE__, a, b);      CURRENT_TEST_PASS = 0; return; }
#define assertGreaterThan(a, b)         if (a <= b)             { ctest_fail("    ctest_fail %s:%d\n    | reason: %d > %d is false\n",      __FILE__, __LINE__, a, b);      CURRENT_TEST_PASS = 0; return; }
#define assertLessThan(a, b)            if (a >= b)             { ctest_fail("    ctest_fail %s:%d\n    | reason: %d < %d is false\n",      __FILE__, __LINE__, a, b);      CURRENT_TEST_PASS = 0; return; }
#define assertGreaterThanOrEqual(a, b)  if (a < b)              { ctest_fail("    ctest_fail %s:%d\n    | reason: %d >= %d is false\n",     __FILE__, __LINE__, a, b);      CURRENT_TEST_PASS = 0; return; }
#define assertLessThanOrEqual(a, b)     if (a > b)              { ctest_fail("    ctest_fail %s:%d\n    | reason: %d <= %d is false\n",     __FILE__, __LINE__, a, b);      CURRENT_TEST_PASS = 0; return; }
#define assertNull(a)                   if (a != NULL)          { ctest_fail("    ctest_fail %s:%d\n    | reason: %p == NULL is false\n",   __FILE__, __LINE__, a);         CURRENT_TEST_PASS = 0; return; }
#define assertNotNull(a)                if (a == NULL)          { ctest_fail("    ctest_fail %s:%d\n    | reason: %p != NULL is false\n",   __FILE__, __LINE__, a);         CURRENT_TEST_PASS = 0; return; }
#define assertStringEqual(a, b)         if (strcmp(a, b) != 0)  { ctest_fail("    ctest_fail %s:%d\n    | reason: %s != %s\n",              __FILE__, __LINE__, a, b);    CURRENT_TEST_PASS = 0; return; }
#define assertStringNotEqual(a, b)      if (strcmp(a, b) == 0)  { ctest_fail("    ctest_fail %s:%d\n    | reason: %s == %s\n",              __FILE__, __LINE__, a, b);    CURRENT_TEST_PASS = 0; return; }
#define assertTrue(expr)                if (!(expr))            { ctest_fail("    ctest_fail %s:%d\n    | reason: (%s) != true\n",          __FILE__, __LINE__, #expr);     CURRENT_TEST_PASS = 0; return; }
#define assertFalse(expr)               if (expr)               { ctest_fail("    ctest_fail %s:%d\n    | reason: (%s) != false\n",         __FILE__, __LINE__, #expr);     CURRENT_TEST_PASS = 0; return; }
#define assertFail                                              { ctest_fail("    ctest_fail %s:%d\n    | reason: forced",                  __FILE__, __LINE__);            CURRENT_TEST_PASS = 0; return; }
#define assertPass                                              { ctest_pass("    ctest_pass\n");                                                                           CURRENT_TEST_PASS = 1; return; }

// exceptions
#define expectException(e) if (fetestexcept(FE_ALL_EXCEPT) != e) { ctest_fail("     ctest_fail %s:%d\n     reason: didn't throw %s\n", __FILE__, __LINE__, #e); CURRENT_TEST_PASS = 0; }
#define expectExceptionNotThrown(e) if (fetestexcept(FE_ALL_EXCEPT) == e) { ctest_fail("     ctest_fail %s:%d\n     reason: threw %s\n", __FILE__, __LINE__, #e); CURRENT_TEST_PASS = 0; }

// timers
#define startTimer TEST_START_TIME = clock()
#define endTimer TEST_END_TIME = clock()
#define deltaTime ((double)(TEST_END_TIME - TEST_START_TIME) / CLOCKS_PER_SEC)
#define printTimer fprintf(test_out, "    time: %f\n", deltaTime)

// user utility functions
#define testPrint(...) fprintf(test_out, "    | "); fflush(test_out); ctest_info(__VA_ARGS__)
#define wait(d) sleep(d)
#define waitms(d) usleep(d * 1000)

//*****************************************************************************
// internal utility functions
// colors
#define ctest_errSetColor(r, g, b) fprintf(stderr, "\033[38;2;%d;%d;%dm", r, g, b)
#define ctest_errReserColor() fprintf(stderr, "\033[0m")
#define ctest_setColor(r, g, b) if (!test_out) { ctest_errSetColor(212, 55, 92); fprintf(stderr, "Set test_out in main: test_out = stderr;\n"); ctest_errReserColor(); exit(1); } if (test_out == stderr || test_out == stdout) fprintf(test_out, "\033[38;2;%d;%d;%dm", r, g, b)
#define ctest_resetColor() if (test_out == stderr || test_out == stdout) fprintf(test_out, "\033[0m")

// formatting
#define ctest_pass(...)     ctest_setColor(0, 189, 142);    fprintf(test_out, __VA_ARGS__); ctest_resetColor()
#define ctest_fail(...)     ctest_setColor(212, 55, 92);    fprintf(test_out, __VA_ARGS__); ctest_resetColor()
#define ctest_group(...)    ctest_setColor(227, 202, 111);  fprintf(test_out, __VA_ARGS__); ctest_resetColor()
#define ctest_end(...)      ctest_setColor(0, 120, 90);     fprintf(test_out, __VA_ARGS__); ctest_resetColor()
#define ctest_info(...)     ctest_setColor(200, 200, 200);  fprintf(test_out, __VA_ARGS__); ctest_resetColor()
#define ctest_intro(...)    ctest_setColor(52, 152, 219);   fprintf(test_out, __VA_ARGS__); ctest_resetColor()