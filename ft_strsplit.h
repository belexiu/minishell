#ifndef FT_STRSPLIT_H
#define FT_STRSPLIT_H

char		**ft_strsplit(char *str, char c);

int			ft_splitlen(char **split_str);

void		ft_splitfree(char **split_str);

#endif
