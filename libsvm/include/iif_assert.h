/** @file iif_assert.h
 *  @brief Provide iif_assert and iif_assume support for system assume and assert macros.
 *
 *  For each valid test, iif_assume and iif_assert should be called only once.
 *  Otherwise, the test is regarded as an invalid test.
 *
 *  @author Li Jiaying
 *  @bug unset_console_color is set the console back to black background, white forground, no strong comparision
 *		instead of the previous setting.
 */
#ifndef _IIF_ASSERT_H_
#define _IIF_ASSERT_H_

/// a flag to justify whether the given input has pass loop precondition
extern bool _passP;
/// a flag to justify whether the given input has pass loop postcondition
extern bool _passQ;

/// integers values contain the call times to iif_assume and iif_assert, used to validate a given test
extern int assume_times, assert_times;

/** @brief Used to envelope loop precondition
 *
 *  Do not support multiple calls
 *
 *  @param expr: loop precondition
 */
#define iif_assume(expr) do { \
	_passP = (expr)? true : false;\
	assume_times++;\
} while(0)

/** @brief Used to envelope loop precondition
 *
 *  Do not support multiple calls
 *  
 *  @param expr: loop postcondition
 */
#define iif_assert(expr) do { \
	_passQ = (expr)? true : false;\
	assert_times++;\
} while(0)

#endif
