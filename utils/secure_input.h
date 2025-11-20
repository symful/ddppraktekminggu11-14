#ifndef SECURE_INPUT_H
#define SECURE_INPUT_H

#include <stddef.h>


#define VERY_WEAK_PASSWORD 0
#define WEAK_PASSWORD 1
#define MEDIUM_PASSWORD 2
#define STRONG_PASSWORD 3



/**
 * Disable terminal echo for secure input
 */
void disableEcho();

/**
 * Enable terminal echo
 */
void enableEcho();

/**
 * Read password securely with asterisk masking
 * @param prompt The prompt to display
 * @param password Buffer to store the password
 * @param maxLength Maximum length of the password buffer
 * @return 1 if successful, 0 if failed
 */
int readSecurePassword(const char *prompt, char *password, size_t maxLength);

/**
 * Read password securely with custom mask character
 * @param prompt The prompt to display
 * @param password Buffer to store the password
 * @param maxLength Maximum length of the password buffer
 * @param maskChar Character to display instead of actual input
 * @return 1 if successful, 0 if failed
 */
int readSecurePasswordWithMask(const char *prompt, char *password,
                               size_t maxLength, char maskChar);

/**
 * Read password silently (no visual feedback)
 * @param prompt The prompt to display
 * @param password Buffer to store the password
 * @param maxLength Maximum length of the password buffer
 * @return 1 if successful, 0 if failed
 */
int readSilentPassword(const char *prompt, char *password, size_t maxLength);

/**
 * Read password with confirmation
 * @param prompt The initial password prompt
 * @param confirmPrompt The confirmation prompt
 * @param password Buffer to store the password
 * @param maxLength Maximum length of the password buffer
 * @return 1 if successful and passwords match, 0 if failed
 */
int readSecurePasswordWithConfirmation(const char *prompt,
                                       const char *confirmPrompt,
                                       char *password, size_t maxLength);

/**
 * Read password with strength checking
 * @param prompt The prompt to display
 * @param password Buffer to store the password
 * @param maxLength Maximum length of the password buffer
 * @return 1 if successful, 0 if failed
 */
int readSecurePasswordWithStrengthCheck(const char *prompt, char *password,
                                        size_t maxLength);

/**
 * Validate password strength
 * @param password The password to validate
 * @return Password strength level (VERY_WEAK_PASSWORD to STRONG_PASSWORD)
 */
int validatePasswordStrength(const char *password);

/**
 * Get password strength description
 * @param strength The strength level
 * @return Human-readable description of the strength
 */
const char *getPasswordStrengthDescription(int strength);

/**
 * Securely clear password from memory
 * @param password The password buffer to clear
 * @param length The length of the buffer
 */
void clearPassword(char *password, size_t length);

/**
 * Read username securely with validation
 * @param prompt The prompt to display
 * @param username Buffer to store the username
 * @param maxLength Maximum length of the username buffer
 * @return 1 if successful, 0 if failed
 */
int readSecureUsername(const char *prompt, char *username, size_t maxLength);

#endif
