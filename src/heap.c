/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakourya <aakourya@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static bool	higher(t_request a, t_request b, t_scheduler scheduler)
{
	if (scheduler == FIFO)
		return (a.key < b.key);
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.id < b.id);
}

static void	swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = calloc(capacity, sizeof(t_request));
	heap->size = 0;
	heap->capacity = capacity;
}

int	heap_push(t_heap *heap, t_request req, t_scheduler scheduler)
{
	int	i;
	int	parent;

	if (heap->size >= heap->capacity)
		return (1);
	i = heap->size++;
	heap->nodes[i] = req;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!higher(heap->nodes[i], heap->nodes[parent], scheduler))
			break ;
		swap(&heap->nodes[i], &heap->nodes[parent]);
		i = parent;
	}
	return (0);
}

int	heap_pop(t_heap *heap)
{
	int	i;
	int	child;
	int	last;

	if (!heap->size)
		return (-1);
	last = --heap->size;
	if (!heap->size)
		return (0);
	heap->nodes[0] = heap->nodes[last];
	i = 0;
	while (1)
	{
		child = i * 2 + 1;
		if (child >= heap->size)
			break ;
		if (child + 1 < heap->size
			&& heap->nodes[child + 1].key < heap->nodes[child].key)
			child++;
		if (heap->nodes[i].key <= heap->nodes[child].key)
			break ;
		swap(&heap->nodes[i], &heap->nodes[child]);
		i = child;
	}
	return (0);
}

int	heap_peek(t_heap *heap)
{
	if (!heap->size)
		return (-1);
	return (heap->nodes[0].id);
}

void	heap_destroy(t_heap *heap)
{
	free(heap->nodes);
	heap->nodes = NULL;
	heap->size = 0;
	heap->capacity = 0;
}