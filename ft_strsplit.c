#include <stdlib.h>

static int	ft_count_words(char *str, char c)
{
	int i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] != c && (i == 0 || str[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*ft_get_word(char *str, char c)
{
	char	*word;
	int		word_len;
	int		i;

	word_len = 0;
	while(str && str[word_len] && str[word_len] != c)
		word_len++;
	word = malloc(sizeof(char) * (word_len + 1));
	i = 0;
	while (str && i < word_len)
	{
		word[i] = str[i];
		i++;
	}
	word[word_len] = '\0';
	return (word);
}

char		**ft_strsplit(char *str, char c)
{
	int		nr_words;
	char	**result;
	int 	i;
	int		word_i;

	nr_words = ft_count_words(str, c);
	result = malloc(sizeof(char*) * (nr_words + 1));
	i = 0;
	word_i = 0;
	while (str && str[i])
	{
		if (str[i] != c && (i == 0 || str[i - 1] == c))
		{
			result[word_i] = ft_get_word(&str[i], c);
			word_i++;
		}
		i++;
	}
	result[nr_words] = NULL;
	return (result);
}

int			ft_splitlen(char **split_str)
{
	int	len;

	len = 0;
	while (split_str && split_str[len] != NULL)
		len++;
	return (len);
}

void		ft_splitfree(char **split_str)
{
	int i;

	i = 0;
	while (split_str && split_str[i] != NULL)
	{
		free(split_str[i]);
		i++;
	}
	free(split_str);
}