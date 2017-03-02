#ifndef GNL_H
# define GNL_H

# define BUFF_SIZE 32

typedef struct		s_node
{
	char			str[BUFF_SIZE];
	int				len;
	struct s_node	*next;
}					t_node;

typedef	struct		s_static_info
{
	int				eof_reached;
	int				fd;
	int				nr_calls;
	t_node			*h;
	t_node			*t;
	int				nr_chr_in_list;
}					t_static_info;

int					ft_gnl(const int fd, char **line);
void				*ft_memmove(void *dst, const void *src, int len);
void				ft_free_list(t_static_info *inf, int ieol);
void				ft_reset_linfo(t_static_info *linfo, int fd);

#endif