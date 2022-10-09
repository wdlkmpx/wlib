/*
 * Public Domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "wlib.h"
#include <glib.h>

// =============================================================

static void test_w_path_get_basename (char *filename)
{
    char *r1, *r2;
    r1 = w_path_get_basename (filename);
    r2 = g_path_get_basename (filename);
    if (filename) {
        puts (filename);
        //puts (r1); puts (r2);
    }
    printf ("r1: %p   r2: %p\n", r1, r2);
    assert (w_strcmp0(r1,r2) == 0);
    w_free (r1);
    w_free (r2);
}

static void test_w_path_get_dirname (char *filename)
{
    char *r1, *r2;
    r1 = w_path_get_dirname (filename);
    r2 = g_path_get_dirname (filename);
    if (filename) {
        puts (filename);
        //puts (r1); puts (r2);
    }
    printf ("r1: %p   r2: %p\n", r1, r2);
    assert (w_strcmp0(r1,r2) == 0);
    w_free (r1);
    w_free (r2);
}


static void w_fileutils_test (void)
{
    puts ("");
    puts ("w_fileutils_test");
    //g_mkdir_with_parents ("a/b/c/d", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    // w_path_get_basename
    printf ("- test_w_path_get_basename\n");
    test_w_path_get_basename ("/path/to/destruction");
    test_w_path_get_basename ("\\path\\to\\destruction");
    test_w_path_get_basename ("nodirs");
    test_w_path_get_basename ("");
    test_w_path_get_basename (NULL);
    printf ("+ test_w_path_get_basename = OK\n");

    // w_path_get_dirname
    printf ("- test_w_path_get_dirname\n");
    test_w_path_get_dirname ("/path/to/destruction");
    test_w_path_get_dirname ("\\path\\to\\destruction");
    test_w_path_get_dirname ("nodirs");
    test_w_path_get_dirname ("");
    test_w_path_get_dirname (NULL);
    printf ("+ test_w_path_get_dirname = OK\n");
}

// =============================================================

static void w_inet_test (void)
{
    puts ("");
    puts ("w_inet_test");
    // w_inet_ipv4_*
    uint32_t ipnum;
    char ipstr[30];
    ipnum = w_inet_ipv4_str_to_num ("255.255.255.255");
    printf ("ipnum: %u\n", ipnum);
    w_inet_ipv4_num_to_str (ipnum, ipstr, sizeof(ipstr));
    printf ("ipstr: %s\n", ipstr);
    assert (strcmp (ipstr, "255.255.255.255") == 0);
    ipnum = w_inet_ipv4_str_to_num ("192.168.1.99");
    printf ("ipnum: %u\n", ipnum);
    w_inet_ipv4_num_to_str (ipnum, ipstr, sizeof(ipstr));
    printf ("ipstr: %s\n", ipstr);
    assert (strcmp (ipstr, "192.168.1.99") == 0);
}

// =============================================================

static void test_w_strreverse (char *str)
{
    char t1[50], t2[50];
    strcpy (t1, str);
    strcpy (t2, str);
    g_strreverse (t1);
    w_strreverse (t2);
    assert (w_strcmp0(t1,t2) == 0);
}

static void test_w_strchug (char *str)
{
    char t1[50], t2[50];
    strcpy (t1, str);
    strcpy (t2, str);
    g_strchug (t1);
    w_strchug (t2);
    puts (t1);
    //printf ("%u - %u\n", strlen(t1), strlen(t2));
    assert (w_strcmp0(t1,t2) == 0);
}

static void test_w_strchomp (char *str)
{
    char t1[50], t2[50];
    strcpy (t1, str);
    strcpy (t2, str);
    puts (t1);
    g_strchomp (t1);
    w_strchomp (t2);
    //printf ("%u - %u\n", strlen(t1), strlen(t2));
    assert (w_strcmp0(t1,t2) == 0);
}

static void w_strfuncs_test (void)
{
    puts ("");
    puts ("w_strfuncs_test");
    char * sarray1[] = { (char*) "a", (char*)"b", (char*)"c", NULL, };
    char * sarray2[] = { (char*) "C", (char*)"D", NULL, };
    char * sarray3[] = { NULL, };
    char * p;
    int ret;
    /* w_strv_length */
    ret = w_strv_length (sarray1);
    assert (ret == 3);
    ret = w_strv_length (sarray2);
    assert (ret == 2);
    ret = w_strv_length (NULL);
    assert (ret == 0);
    ret = w_strv_length (sarray3);
    assert (ret == 0);

    /* w_strconcat */
    p = w_strconcat ("", "a", "b", "", "", "c", "", "", "-", NULL);
    puts (p);
    assert (p != NULL);
    assert (strcmp(p,"abc-") == 0);
    free (p);

    test_w_strreverse ("abcdefg");
    test_w_strreverse ("a");
    test_w_strreverse ("");
    g_strreverse (NULL);
    w_strreverse (NULL);

    test_w_strchug ("  hey hey  ");
    test_w_strchug ("  aaaa  ");
    test_w_strchug ("  \tkeaton1  ");
    test_w_strchug ("  \nkeaton2  ");
    //test_w_strchug ("  \vkeaton3 "); // this fails with isspace(), it's not removed by glib
    test_w_strchug ("  \fkeaton4  ");
    test_w_strchug ("  \rkeaton5  ");
    test_w_strchug ("  \1irc_msg  ");
    test_w_strchug ("   ");
    test_w_strchug (" ");
    test_w_strchug ("");
    w_strchug (NULL);
    g_strchug (NULL);

    test_w_strchomp ("  hey hey  ");
    test_w_strchomp ("  aaaa  ");
    test_w_strchomp ("  \tkeaton1  ");
    test_w_strchomp ("  \nkeaton2  ");
    //test_w_strchomp ("  \vkeaton3 "); // this fails with isspace(), it's not removed by glib
    test_w_strchomp ("  \fkeaton4  ");
    test_w_strchomp ("  \rkeaton5  ");
    test_w_strchomp ("  irc_msg\1  ");
    test_w_strchomp ("   ");
    test_w_strchomp (" ");
    test_w_strchomp ("");
    w_strchomp (NULL);
    g_strchomp (NULL);
}

// =============================================================

static void test_w_find_program_in_path (char *app)
{
    char *r1;
    char *r2;
    r1 = w_find_program_in_path (app);
    r2 = g_find_program_in_path (app);
    assert (w_strcmp0(r1,r2) == 0);
    if (r1) puts (r1);
    w_free (r1);
    w_free (r2);
}

static void w_utils_test (void)
{
    char *path = strdup (getenv ("PATH"));
    puts ("");
    puts ("w_utils_test");
    printf ("- test_w_find_program_in_path\n");
    test_w_find_program_in_path ("ls");
    test_w_find_program_in_path ("mplayerX");
    test_w_find_program_in_path ("");
    unsetenv ("PATH");
    test_w_find_program_in_path ("ls");
    setenv ("PATH", "", 1);
    test_w_find_program_in_path ("ls");
    setenv ("PATH", "/usr/local/bin:/sbin", 1);
    test_w_find_program_in_path ("ls");
    // restore PATH
    setenv ("PATH", path, 1);
    test_w_find_program_in_path ("mkdir");
    //test_w_find_program_in_path (NULL);
    printf ("+ test_w_find_program_in_path\n");
}

// =============================================================

int main (int argc, char ** argv)
{
    w_fileutils_test ();
    w_inet_test ();
    w_strfuncs_test ();
    w_utils_test ();
    puts ("");
    puts ("All OK");
    return 0;
}
