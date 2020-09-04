#include "dtls_bio.h"
#include <openssl/bio.h>
#include <openssl/err.h>

static int mtu = 800;
static dtls_bio_agent_write_callback_t cb_write = NULL;
static BIO_METHOD *dtls_bio_agent_methods = NULL;

static int bio_agent_new(BIO *bio)
{
    BIO_set_init(bio, 1);
    BIO_set_data(bio, NULL);
    BIO_set_shutdown(bio, 0);
    return 1;
}

static int bio_agent_free(BIO *bio)
{
    if (bio == NULL)
        return 0;
    BIO_set_data(bio, NULL);
    return 1;
}

static int bio_agent_write(BIO *bio, const char *in, int inl)
{
    int bytes = -1;
    if (inl <= 0) {
        printf("bio_agent_write failed: %d\n", inl);
        return inl;
    }
    printf("bio_agent_write: %p, %d\n", in, inl);

    void *ctx = BIO_get_data(bio);
    if (cb_write) {
        bytes = cb_write(ctx, in, inl);
    }

    return bytes;
}

static long bio_agent_ctrl(BIO *bio, int cmd, long num, void *ptr)
{
    (void)bio;
    (void)num;
    (void)ptr;
    switch (cmd) {
    case BIO_CTRL_FLUSH:
        return 1;
    case BIO_CTRL_DGRAM_QUERY_MTU:
        printf("bio_agent_ctrl, query MTU: %d\n", mtu);
        return mtu;
    case BIO_CTRL_WPENDING:
    case BIO_CTRL_PENDING:
        return 0L;
    case BIO_CTRL_DGRAM_GET_MTU_OVERHEAD:
        break;
    default:
        printf("bio_agent_ctrl: %d\n", cmd);
    }
    return 0;
}

int dtls_bio_agent_init(dtls_bio_agent_write_callback_t cb)
{
    dtls_bio_agent_methods =
        BIO_meth_new(BIO_TYPE_FILTER | BIO_get_new_index(), "dtls-bio");
    if (!dtls_bio_agent_methods)
        return -1;
    BIO_meth_set_create(dtls_bio_agent_methods, bio_agent_new);
    BIO_meth_set_destroy(dtls_bio_agent_methods, bio_agent_free);
    BIO_meth_set_write(dtls_bio_agent_methods, bio_agent_write);
    BIO_meth_set_ctrl(dtls_bio_agent_methods, bio_agent_ctrl);
    cb_write = cb;
    return 0;
}

BIO *dtls_bio_agent_new(void *ctx)
{
    BIO *bio = BIO_new(dtls_bio_agent_methods);
    if (bio == NULL) {
        return NULL;
    }
    BIO_set_data(bio, ctx);
    return bio;
}

void dtls_bio_agent_destroy(void)
{
    cb_write = NULL;
    if (dtls_bio_agent_methods)
        BIO_meth_free(dtls_bio_agent_methods);
}
