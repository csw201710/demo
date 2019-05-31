/*
 * configfile.h
 *
 * Function to read pptpd config file.
 */

#ifndef _PPTPD_CONFIGFILE_H
#define _PPTPD_CONFIGFILE_H

int read_config_file(char *filename, char *keyword, char *value);

#define MAX_CONFIG_STRING_SIZE 1024
#endif  /* !_PPTPD_CONFIGFILE_H */
