#include "mgp_internal.h"

t_mgp		mgp_init()
{
	t_mgp	mgp;

	bzero(&mgp, sizeof(t_mgp));
	return mgp;
}
