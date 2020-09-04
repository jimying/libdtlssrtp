#ifndef DTLS_BIO_H_
#define DTLS_BIO_H_

// OpenSSL BIO writes packets

// bio write callback
typedef int (*dtls_bio_agent_write_callback_t)(void *ctx, const char *data,
                                               int len);

int dtls_bio_agent_init(dtls_bio_agent_write_callback_t cb);
void dtls_bio_agent_destroy(void);
BIO *dtls_bio_agent_new(void *ctx);

#endif // DTLS_BIO_H_
