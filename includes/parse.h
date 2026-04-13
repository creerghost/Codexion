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
 * Supported dongle arbitration policies.
 */
typedef enum e_scheduler
{
	CODEX_SCHED_FIFO = 0,
	CODEX_SCHED_EDF = 1
}				t_scheduler;

/**
 * Runtime configuration parsed from CLI arguments.
 */
typedef struct s_config
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}				t_config;

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