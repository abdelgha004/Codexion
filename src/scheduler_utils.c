/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 05:53:58 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/10 07:25:52 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	swap_nodes(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heapify_up(t_heap *heap, int index, int scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (is_higher_priority(heap->nodes[index], heap->nodes[parent],
				scheduler))
		{
			swap_nodes(&heap->nodes[index], &heap->nodes[parent]);
			index = parent;
		}
		else
			break ;
	}
}

void	heapify_down(t_heap *heap, int index, int scheduler)
{
	int	left;
	int	right;
	int	winner;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		winner = index;
		if (left < heap->size && is_higher_priority(heap->nodes[left],
				heap->nodes[winner], scheduler))
			winner = left;
		if (right < heap->size && is_higher_priority(heap->nodes[right],
				heap->nodes[winner], scheduler))
			winner = right;
		if (winner != index)
		{
			swap_nodes(&heap->nodes[index], &heap->nodes[winner]);
			index = winner;
		}
		else
			break ;
	}
}
