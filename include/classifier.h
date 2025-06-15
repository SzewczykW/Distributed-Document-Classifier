/**
 * @file classifier.h
 * @author Wiktor Szewczyk
 * @brief Tokenization and text classification logic.
 */

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

/// Token delimiters for splitting text into words
#define CLASSIFY_TOKENS " \n\t.,;:!?()[]{}\"'-"

/**
 * @brief Generates a classification vector from input text.
 *
 * The function tokenizes the input string and compares each token
 * to a global hash table containing dictionary keywords.
 *
 * @param text Input document text
 * @param result Output vector (must be zeroed and sized to keyword_count)
 * @param keyword_count Number of keywords in dictionary
 */
void classify_text(const char *text, int result[], int keyword_count);

#endif
