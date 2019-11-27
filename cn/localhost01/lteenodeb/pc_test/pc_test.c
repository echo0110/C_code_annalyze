#include "pc_test.h"
void*
rrm_msg_mem_get
(
 rrm_size_t size /* Size of buffer which will be allocated */
 )
{
	return malloc(size);
}
void
rrm_msg_mem_free
(
	void* p_buffer /* Pointer to buffer which will be freed */
)
{
	free(p_buffer);
}

void*
rrc_msg_mem_get
(
	rrc_size_t size /* Size of buffer which will be allocated */
)
{
	return malloc(size);
}
void
rrc_msg_mem_free
(
	void* p_buffer /* Pointer to buffer which will be freed */
)
{
	free(p_buffer);
}

void* oam_msg_mem_get(size_t size)/* Size of buffer which will be allocated */
{
	return malloc(size);
}
void *oam_memset(void *s, int c, size_t n)
{
        return memset(s, c, n);
}
oam_return_et
oam_mem_free
(
 void *p_buffer, /* Pointer to buffer which will be freed */
 oam_error_code_et *p_error_code
 )
{
	free(p_buffer);
	return OAM_SUCCESS ;
}

