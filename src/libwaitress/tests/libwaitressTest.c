
/* test cases for libwaitress */


#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"



#include "../waitress.c"

#define streq(a,b) (strcmp(a,b) == 0)

/*	string equality test (memory location or content)
 */
static bool streqtest (const char *x, const char *y) {
	return (x == y) || (x != NULL && y != NULL && streq (x, y));
}

/*	test WaitressSplitUrl
 *	@param tested url
 *	@param expected user
 *	@param expected password
 *	@param expected host
 *	@param expected port
 *	@param expected path
 */
static bool compareUrl (const char *url, const char *user,
		const char *password, const char *host, const char *port,
		const char *path) {
	WaitressUrl_t splitUrl;

	memset (&splitUrl, 0, sizeof (splitUrl));

	WaitressSplitUrl (url, &splitUrl);

	bool userTest, passwordTest, hostTest, portTest, pathTest, overallTest;

	userTest = streqtest (splitUrl.user, user);
	passwordTest = streqtest (splitUrl.password, password);
	hostTest = streqtest (splitUrl.host, host);
	portTest = streqtest (splitUrl.port, port);
	pathTest = streqtest (splitUrl.path, path);

	overallTest = userTest && passwordTest && hostTest && portTest && pathTest;

	if (!overallTest) {
		printf ("FAILED test(s) for %s\n", url);
		if (!userTest) {
			printf ("user: %s vs %s\n", splitUrl.user, user);
		}
		if (!passwordTest) {
			printf ("password: %s vs %s\n", splitUrl.password, password);
		}
		if (!hostTest) {
			printf ("host: %s vs %s\n", splitUrl.host, host);
		}
		if (!portTest) {
			printf ("port: %s vs %s\n", splitUrl.port, port);
		}
		if (!pathTest) {
			printf ("path: %s vs %s\n", splitUrl.path, path);
		}
	} else {
		printf ("OK for %s\n", url);
	}

	return overallTest;
}

/*	compare two strings
 */
bool compareStr (const char *result, const char *expected) {
	if (!streq (result, expected)) {
		printf ("FAIL for %s, result was %s\n", expected, result);
		return false;
	} else {
		printf ("OK for %s\n", expected);
		return true;
	}
}

/*-------------------------------------------------------------------------*
 * libwaitress Test
 *-------------------------------------------------------------------------*/

void TestWaitress(CuTest* tc) {
	/* WaitressSplitUrl tests */
	CuAssertTrue(tc,compareUrl ("http://www.example.com/", NULL, NULL, "www.example.com", NULL,
			""));
	CuAssertTrue(tc,compareUrl ("http://www.example.com", NULL, NULL, "www.example.com", NULL,
			NULL));
	CuAssertTrue(tc,compareUrl ("http://www.example.com:80/", NULL, NULL, "www.example.com",
			"80", ""));
	CuAssertTrue(tc,compareUrl ("http://www.example.com:/", NULL, NULL, "www.example.com", "",
			""));
	CuAssertTrue(tc,compareUrl ("http://:80/", NULL, NULL, "", "80", ""));
	CuAssertTrue(tc,compareUrl ("http://www.example.com/foobar/barbaz", NULL, NULL,
			"www.example.com", NULL, "foobar/barbaz"));
	CuAssertTrue(tc,compareUrl ("http://www.example.com:80/foobar/barbaz", NULL, NULL,
			"www.example.com", "80", "foobar/barbaz"));
	CuAssertTrue(tc,compareUrl ("http://foo:bar@www.example.com:80/foobar/barbaz", "foo", "bar",
			"www.example.com", "80", "foobar/barbaz"));
	CuAssertTrue(tc,compareUrl ("http://foo:@www.example.com:80/foobar/barbaz", "foo", "",
			"www.example.com", "80", "foobar/barbaz"));
	CuAssertTrue(tc,compareUrl ("http://foo@www.example.com:80/foobar/barbaz", "foo", NULL,
			"www.example.com", "80", "foobar/barbaz"));
	CuAssertTrue(tc,compareUrl ("http://:foo@www.example.com:80/foobar/barbaz", "", "foo",
			"www.example.com", "80", "foobar/barbaz"));
	CuAssertTrue(tc,compareUrl ("http://:@:80", "", "", "", "80", NULL));
	CuAssertTrue(tc,compareUrl ("http://", NULL, NULL, NULL, NULL, NULL));
	CuAssertTrue(tc,compareUrl ("http:///", NULL, NULL, "", NULL, ""));
	CuAssertTrue(tc,compareUrl ("http://foo:bar@", "foo", "bar", "", NULL, NULL));

	/* WaitressBase64Encode tests */
	CuAssertTrue(tc,compareStr (WaitressBase64Encode ("M"), "TQ=="));
	CuAssertTrue(tc,compareStr (WaitressBase64Encode ("Ma"), "TWE="));
	CuAssertTrue(tc,compareStr (WaitressBase64Encode ("Man"), "TWFu"));
	CuAssertTrue(tc,compareStr (WaitressBase64Encode ("The quick brown fox jumped over the lazy dog."),
			"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wZWQgb3ZlciB0aGUgbGF6eSBkb2cu"));
	CuAssertTrue(tc,compareStr (WaitressBase64Encode ("The quick brown fox jumped over the lazy dog"),
			"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wZWQgb3ZlciB0aGUgbGF6eSBkb2c="));
	CuAssertTrue(tc,compareStr (WaitressBase64Encode ("The quick brown fox jumped over the lazy do"),
			"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wZWQgb3ZlciB0aGUgbGF6eSBkbw=="));

}



CuSuite* CuGetSuite(void)
{
	CuSuite* suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, TestWaitress);
	
	return suite;
}


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestWaitress);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}


/*-------------------------------------------------------------------------*
 * main
 *-------------------------------------------------------------------------*/
int main(void)
{
    RunAllTests();
    return 0;
}
