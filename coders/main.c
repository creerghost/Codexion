/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 13:59:20 by vlnikola          #+#    #+#             */
/*   Updated: 2026/04/13 15:20:13 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "parse.h"
#include "init.h"
#include "queue.h"

int	main(int argc, char **argv)
{
	t_config	config;
	t_sim		sim;

	if (!parse_args(argc, argv, &config))
		return (1);
	if (!init_sim(&sim, &config))
		return (1);
	return (0);
}
