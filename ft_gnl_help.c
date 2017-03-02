#include "ft_gnl.h"
#include <stdlib.h>

void	ft_free_list(t_static_info *inf, int ieol)
{
	t_node	*crowler;
	t_node	*next;

	if (ieol <= inf->t->len - 2)
	{
		ft_memmove(inf->h->str, inf->t->str + ieol + 1, inf->t->len - ieol - 1);
		inf->nr_chr_in_list = inf->t->len - ieol - 1;
		inf->h->len = inf->t->len - ieol - 1;
		crowler = inf->h->next;
	}
	else
	{
		crowler = inf->h;
		inf->nr_chr_in_list = 0;
		inf->h = NULL;
	}
	while (crowler != NULL)
	{
		next = crowler->next;
		free(crowler);
		crowler = next;
	}
	if (inf->h)
		inf->h->next = NULL;
	inf->t = inf->h;
}

void	ft_reset_linfo(t_static_info *linfo, int fd)
{
	if (linfo->h != NULL)
		ft_free_list(linfo, linfo->t->len);
	linfo->nr_calls = 0;
	linfo->nr_chr_in_list = 0;
	linfo->fd = fd;
	linfo->h = NULL;
	linfo->t = NULL;
	linfo->eof_reached = 0;
}

void	*ft_memmove(void *dst, const void *src, int len)
{
	char			*d;
	const	char	*s;

	d = dst;
	s = src;
	if (s > d)
	{
		while (len > 0)
		{
			*d = *s;
			d++;
			s++;
			len--;
		}
	}
	else
	{
		while (len > 0)
		{
			d[len - 1] = s[len - 1];
			len--;
		}
	}
	return (dst);
}