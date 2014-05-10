
#ifndef __included_terra_string_h
#define __included_terra_string_h

#include "tdefine.h"
#include <ctype.h>

inline char *TStringLwr(char *s) {
	char *p = s;
	for (; *p; p++)
		*p = tolower(*p);
	return s;
}

inline char *TStringUpr(char *s) {
	char *p = s;
	for (; *p; p++)
		*p = toupper(*p);
	return s;
}

int TStringAdjustSize(char **text,size_t oldsize,size_t newsize);

size_t TStringRCSpn(const char *_str,const char *_control);

size_t TStringNumOccurences(const char *target,const char *match);

/**
 * Replace a substring of a string with another string
 *
 * @param source              The string to modify.
 * @param match               The substring to be matched.
 * @param replacement         The string replacing the match.
 * @param hint_numoccurence   The number of occurence of the match in the string.
 *                            This value can be 0 and is useless if the match length
 *                            equals the replacement length.
 * @return                    A new string with the matching strings replaced.
 *
 * @sa TStringReplaceInplace
 */
char *TStringReplace(const char *source, const char *match, const char *replacement, size_t hint_numoccurence);

/**
 * Replace a substring of a string with another string in place.
 * The match and replacement must be of the same size.
 *
 * @param source              The string to modify.
 * @param match               The substring to be matched.
 * @param replacement         The string replacing the matches.
 * @return                    0 on success,
 *                            1 if a parameter is invalid,
 *                            2 for a length mismatch.
 *
 * @sa TStringReplace
 */
unsigned char TStringReplaceInplace(char *source, const char *match, const char *replacement);

void TSTringSafetyString(char *string);

char **TSTringSplit(const char *string, const char *substr, size_t *size);
char *TSTringAddCharacter(const char *string, char character, size_t start, size_t end);

void TSTringStripTrailingWhitespace(char *string);	// Removes trailing /n, /r, space
char *TSTringLowerCase(const char *thestring);

char *TSTringAseEncrypt(const char * src, unsigned char key);
char *TSTringPasswordEncrypt(const char *src);

char *TSTringDoubleChars(const char *string, const char escchar);
char *TSTringRemoveDuplication(const char *string, const char escchar);

gint TStringCompareGlibFull(gconstpointer a,gconstpointer b,gpointer c);
inline gint TStringCompareGlib(gconstpointer a,gconstpointer b)
{
	return TStringCompareGlibFull(a,b,0);
}

#endif
