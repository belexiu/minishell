#include <stdlib.h>
#include <unistd.h>
#include "ft_gnl.h"

int		ft_get_i_eol(t_static_info *linfo)
{
	int	i;

	if (linfo->t == NULL)
		return (-1);
	i = 0;
	while (i < linfo->t->len)
	{
		if (linfo->t->str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_glue(t_static_info *list, int index_eol)
{
	char	*result;
	char	*tmp;
	t_node	*crowler;
	int		size;

	if (list->h == list->t)
		size = index_eol;
	else
		size = list->nr_chr_in_list - (list->t->len - index_eol - 1);
	result = malloc(sizeof(char) * (size + 1));
	tmp = result;
	crowler = list->h;
	while (crowler != NULL)
	{
		if (crowler == list->t)
			size = index_eol;
		else
			size = crowler->len;
		ft_memmove(tmp, crowler->str, size);
		tmp += size;
		crowler = crowler->next;
	}
	*tmp = '\0';
	return (result);
}

int		ft_try_build_line(t_static_info *linfo, char **dest)
{
	int i_eol;
	int ret_val;

	i_eol = ft_get_i_eol(linfo);
	if (linfo->t && linfo->t->len == -1)
		ret_val = -1;
	else if (linfo->t && linfo->h->len == 0 && linfo->eof_reached)
		ret_val = 0;
	else if (linfo->h &&
			 ((linfo->t->len < BUFF_SIZE && linfo->h != linfo->t) ||
			  i_eol != -1 || linfo->eof_reached))
	{
		ret_val = 1;
		if (i_eol == -1)
			i_eol = linfo->t->len;
	}
	else
		ret_val = 2;
	if (ret_val == 1 || ret_val == 0)
	{
		*dest = ft_glue(linfo, i_eol);
		ft_free_list(linfo, i_eol);
	}
	return (ret_val);
}

void	ft_add_to_list(t_static_info *linfo, t_node *new_node)
{
	new_node->next = NULL;
	linfo->nr_chr_in_list += new_node->len;
	if (linfo->h == NULL)
	{
		linfo->h = new_node;
		linfo->t = new_node;
	}
	else
	{
		linfo->t->next = new_node;
		linfo->t = new_node;
	}
}

int		ft_gnl(const int fd, char **line)
{
	static t_static_info	linfo;
	int						status;
	t_node					*new_node;

	if (fd < 0 || line == NULL)
		return (-1);
	if (fd != linfo.fd)
		ft_reset_linfo(&linfo, fd);
	while ((status = ft_try_build_line(&linfo, line)) > 1)
	{
		new_node = malloc(sizeof(t_node));
		new_node->len = read(fd, new_node->str, BUFF_SIZE);
		if (new_node->len == 0)
			linfo.eof_reached = 1;
		ft_add_to_list(&linfo, new_node);
	}
	return (status);
}