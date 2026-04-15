#include "transmit.hpp"
#include "Utils/logging.hpp"
#include "notecard.hpp"

static const char TAG[] = "transmit";

void Transmit::Init() { Notecard::Init(); }

void Transmit::Setup()
{
    Notecard::Configure(24 * 60); /* 1/day */
    Notecard::Sync(300000);       /* 5 minutes on first boot */
}

void Transmit::EnQueue(record_t *record)
{
    Notecard::Send(record, false);
    LOG_INF(TAG, "Enqueued newest record for transmission.");
}

void Transmit::Notify(power_event_t *event)
{
    Notecard::Send(event, true);
    LOG_INF(TAG, "Send power event notification.");
}