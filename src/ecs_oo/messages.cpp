#include "messages.hpp"

const base_message::id_type death_message::id = base_message::id_gen::get<death_message>();
const base_message::id_type health_message::id = base_message::id_gen::get<health_message>();
const base_message::id_type position_changed_message::id = base_message::id_gen::get<position_changed_message>();
