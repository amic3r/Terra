
#ifndef __included_terra_string_h
#define __included_terra_string_h

_inline char *strlwr(char *s) {
	char *p = s;
	for (; *p; p++)
		*p = tolower(*p);
	return s;
}

_inline char *strupr(char *s) {
	char *p = s;
	for (; *p; p++)
		*p = toupper(*p);
	return s;
}

__inline char *string_adjust_size(char *text,unsigned int oldsize,unsigned int newsize)
{
	char *biggerline = (char*) realloc(text, newsize);
	if(!biggerline) return 0;

	if(newsize > oldsize)
		memset(biggerline+oldsize,'\0',newsize);

	return biggerline;
}

unsigned int strrcspn(const char *_str,const char *_control);

unsigned int string_num_occurences(const char *target,const char *match);

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
 * @sa string_replace_inplace
 */
char *string_replace(const char *source, const char *match, const char *replacement, unsigned int hint_numoccurence);

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
 * @sa string_replace
 */
unsigned char string_replace_inplace(char *source, const char *match, const char *replacement);

void SafetyString(char *string);

int string_split(SList *sto, const char *string, const char *substr);
char *add_character_to_string(const char *string, char character, unsigned int start, unsigned int end);

void StripTrailingWhitespace(char *string);										// Removes trailing /n, /r, space
char *LowerCaseString(const char *thestring);

char *string_ase_encrypt(const char * src, unsigned char key);
char *string_password_encrypt(const char *src);

char *double_char_in_string(const char *string, const char escchar);
char *remove_character_duplication(const char *string, const char escchar);

gint string_compare_glib_full(gconstpointer a,gconstpointer b,gpointer c);
_inline gint string_compare_glib(gconstpointer a,gconstpointer b)
{
	return string_compare_glib_full(a,b,0);
}

#endif
