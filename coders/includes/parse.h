/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:19:39 by vlnikola          #+#    #+#             */
/*   Updated: 2026/04/13 15:26:46 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "codexion.h"

/**
 * Mapping entry between a config integer field and its argument label.
 */
typedef struct s_int_arg
{
	int			*field;
	const char	*label;
}				t_int_arg;

int				parse_args(int argc, char **argv, t_config *config);

#endif