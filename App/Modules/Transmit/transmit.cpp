#include "transmit.hpp"
#include "Utils/logging.hpp"
#include "notecard.hpp"

static const char TAG[] = "transmit";

void Transmit::Init() { Notecard::Init(); }

void Transmit::Setup() { Notecard::Configure(24 * 60); /* 1/day */ }

void Transmit::EnQueue(record_t *record)
{
    Notecard::Send(record);
    LOG_INF(TAG, "Enqueued newest record for transmission.");
}