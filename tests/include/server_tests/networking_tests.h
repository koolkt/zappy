#ifndef SNCUNIT_TEST_H
#define SNCUNIT_TEST_H

#include "CUnit/Automated.h"
#include "CUnit/Basic.h"

/*
 * Initialise test suite.
 * @return zero on success, non-zero otherwise.
 */
int initialise_suite(void);

/* 
 * Clean up test suite.
 * @return zero on success, non-zero otherwise.
 */
int cleanup_suite(void);

void		test_naive_parser();

#endif
