/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 07:03:23 by aakourya          #+#    #+#             */
/*   Updated: 2026/09/11 20:19:22 by aakourya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = malloc(sizeof(t_request) * capacity);
	if (!heap->nodes)
	{
		heap->size = -1;
		heap->capacity = 0;
		return ;
	}
	heap->size = 0;
	heap->capacity = capacity;
}

int	is_higher_priority(t_request first_req, t_request second_req, int scheduler)
{
	if (!scheduler)
	{
		if (first_req.enter_time != second_req.enter_time)
			return (first_req.enter_time < second_req.enter_time);
		return (first_req.id < second_req.id);
	}
	if (first_req.time_to_burnout != second_req.time_to_burnout)
		return (first_req.time_to_burnout < second_req.time_to_burnout);
	if (first_req.enter_time != second_req.enter_time)
		return (first_req.enter_time < second_req.enter_time);
	return (first_req.id < second_req.id);
}

void	add_request(t_heap *heap, t_request *node, int scheduler)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->nodes[heap->size] = *node;
	heap->size++;
	move_node_up(heap, heap->size - 1, scheduler);
}

t_request	remove_top_request(t_heap *heap, int scheduler)
{
	t_request	res;

	heap->size--;
	res = heap->nodes[0];
	heap->nodes[0] = heap->nodes[heap->size];
	move_node_down(heap, 0, scheduler);
	return (res);
}

void	remove_request(t_heap *heap, int id, int scheduler)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->nodes[i].id == id)
		{
			remove_heap_node(heap, i, scheduler);
			return ;
		}
		i++;
	}
}
