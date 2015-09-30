/*
 * TS is abbreviation for Terminal Style.
 * */

#ifndef TS
#define TS 1  

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#define TS_RESET "\x1b[0m"

/* ATTRIBUTES {{{ */
#define TS_BOLD "\x1b[1m"
/* }}} */

/* COLORS {{{ */
#define TS_YELLOW "\x1b[33m"
#define TS_RED "\x1b[31m"
#define TS_GREEN "\x1b[32m"
#define TS_BLUE "\x1b[34m"
#define TS_BLACK "\x1b[30m"
#define TS_WHITE "\x1b[37m"
#define TS_PURPLE "\x1b[35m"
/* }}} */

/* ARROWS {{{ */
#define TS_ARROW_DOWN_RIGHT "\u21b3"
#define TS_ARROW_DOWN_LEFT "\u21b2"
#define TS_ARROW_UP_RIGHT "\u21b1"
#define TS_ARROW_UP_LEFT "\u21b0"

#define TS_ARROW_RIGHT_DOWN "\u2b0e"
#define TS_ARROW_LEFT_DOWN "\u2b10"
#define TS_ARROW_RIGHT_UP "\u2b0f"
#define TS_ARROW_LEFT_UP "\u2b11"

#define TS_ARROW_DOWN "\u2193"
#define TS_ARROW_UP "\u2191"
#define TS_ARROW_LEFT "\u2190"
#define TS_ARROW_RIGHT "\u2192"
/* }}} */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* TS */