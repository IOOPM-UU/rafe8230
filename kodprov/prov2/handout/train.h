#pragma once

#include <stdbool.h>

/**
 * @typedef track_t
 * @brief Represents a single segment of railway track.
 */
typedef struct track track_t;

/**
 * @brief Creates a new track segment.
 *
 * @param next Pointer to the next track segment, or NULL if this is the end of the line.
 * @return A pointer to the newly allocated track
 */
track_t *track_create(track_t *next);

/**
 * @brief Frees all memory reachable from the given track segment, including switches
 *
 * @param track Pointer to the first segment to free.
 */
void track_destroy(track_t *track);

/**
 * @brief Adds a switch from one segment to another, unless a switch already exists
 *
 * @param track The track segment where the switch starts.
 * @param to   The segment where the switch leads.
 * @return `true` if the switch was successfully added, otherwise `false`.
 */
bool track_add_switch(track_t *track, track_t *to);

/**
 * @brief Checks whether there is a train on the given track segment.
 *
 * @param track The track segment to check.
 * @return `true` if a train occupies the segment, otherwise `false`.
 */
bool track_has_train(track_t *track);

/**
 * @brief Moves a train along the track a given number of steps.
 *
 * Moves a train `steps` segments forward starting from `start`.
 * If a train already occupies `start`, it moves that train;
 * otherwise, a train is placed there before moving.
 *
 * If the parameter `take_switch` is `true`, the train follows any
 * available switches it encounters; otherwise, it continues straight.
 *
 * If the next segment in the intended direction already has a train,
 * the train stops at the current segment.
 *
 * If the train reaches the end of the line, it stops at the last segment.
 *
 * @param start The starting segment for the train.
 * @param steps The number of steps to move forward.
 * @param take_switch Whether to take available switches during movement.
 * @return A pointer to the track segment where the train ends up.
 */
track_t *track_move(track_t *start, size_t steps, bool take_switch);
