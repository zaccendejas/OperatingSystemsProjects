// MutexCheckerMacros.h by Zac Cendejas && Will Fritz
// Automatically causes mutex and condition variable related functions to check for error

#pragma once

#define DID_IT_WORK(f) \
	if (int(f) != 0) { \
		fprintf(stderr, "%s\n", strerror(int(f))); \
		exit(EXIT_FAILURE); \
	}

#define Pthread_mutex_init(l, a) \
    DID_IT_WORK(pthread_mutex_init(l, a));

#define Pthread_mutex_lock(l) \
    DID_IT_WORK(pthread_mutex_lock((pthread_mutex_t *)(l)));

#define Pthread_mutex_unlock(l) \
    DID_IT_WORK(pthread_mutex_unlock((pthread_mutex_t *)(l)));

#define Pthread_cond_init(c, a) \
    DID_IT_WORK(pthread_cond_init((c), (a)));

#define Pthread_cond_wait(c, l) \
    DID_IT_WORK(pthread_cond_wait((c), (l)));

#define Pthread_cond_signal(c) \
    DID_IT_WORK(pthread_cond_signal((pthread_cond_t *)(c)));
