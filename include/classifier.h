#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#define CLASSIFY_TOKENS " \n\t.,;:!?()[]{}\"'-"

void classify_text(const char *text, int result[], int keyword_count);

#endif
