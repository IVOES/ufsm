#include <stdio.h>
#include <assert.h>
#include <ufsm.h>

#include "simple.h"

enum events {
    EV_A,
    EV_B
};

static struct ufsm_state A;

static struct ufsm_state simple_INIT =
{
    .name = "Init",
    .kind = UFSM_STATE_INIT,
    .next = &A
};

static struct ufsm_state B = 
{
    .name = "State B",
    .kind = UFSM_STATE_SIMPLE,
    .next = NULL,
};

static struct ufsm_state A = 
{
    .name = "State A",
    .kind = UFSM_STATE_SIMPLE,
    .next = &B,
};




static struct ufsm_transition simple_transition_B = 
{
    .name = "EV_B",
    .trigger = EV_B,
    .kind = UFSM_TRANSITION_EXTERNAL,
    .source = &A,
    .dest = &B,
    .next = NULL
};


static struct ufsm_transition simple_transition_A = 
{
    .name = "EV_A",
    .trigger = EV_A,
    .kind = UFSM_TRANSITION_EXTERNAL,
    .source = &B,
    .dest = &A,
    .next = &simple_transition_B
};

static struct ufsm_transition simple_transition_INIT = 
{
    .name = "Init",
    .kind = UFSM_TRANSITION_EXTERNAL,
    .source = &simple_INIT,
    .trigger = UFSM_NO_TRIGGER,
    .dest = &A,
    .next = &simple_transition_A,
};

static struct ufsm_region region1 = 
{
    .state = &A,
    .transition = &simple_transition_INIT,
    .next = NULL
};

static struct ufsm_machine m  = 
{
    .name = "Simple Test Machine",
    .region = &region1,
};

bool test_simple() {
    bool test_ok = true;
    uint32_t err;

    err = ufsm_init(&m);
    assert (err == UFSM_OK && "Initializing");
    assert (m.region->current == &A);
    err = ufsm_process(&m, EV_B);
    assert (m.region->current == &B && err == UFSM_OK);
    err = ufsm_process(&m, EV_A);
    assert (m.region->current == &A && err == UFSM_OK);
    err = ufsm_process(&m, EV_B);
    assert (m.region->current == &B && err == UFSM_OK);
    err = ufsm_process(&m, EV_B);
    assert (m.region->current == &B && err == UFSM_ERROR);

    return test_ok;
}
