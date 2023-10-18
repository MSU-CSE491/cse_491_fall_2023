# Group 7 GP Agent


Current progress:
- [x] Sensors for the agent
  - [ ] provide fixes for the sensors <span style="background-color: #FF6347; color: #FFFFFF; padding: 5px; border-radius: 10px;">Fixes</span>
  - [ ] make sensors agnostic to wall type. 
    - make it so that the sensors can detect walls/impenetrable object of any type
    - <span style="background-color: #4CAF50; color: #FFFFFF; padding: 5px; border-radius: 10px;">Integration</span> with Team 1's Library
  - [x] tests for the sensors
- [x] Registers for the agent
  - [x] provide fixes for the registers <span style="background-color: #FF6347; color: #FFFFFF; padding: 5px; border-radius: 10px;">Fixes</span>
  - [x] tests for the registers

Currently working on:
- [ ] LGP agent <span style="background-color: #66CDAA; color: #FFFFFF; padding: 5px; border-radius: 10px;">Released Experimentally</span>
    - [ ] LGP agent tests ?? 
- [ ] CGP agent <span style="background-color: #FFFF00; color: #000000; padding: 5px; border-radius: 10px;">In Development</span> and close to experimental.
    - [ ] CGP agent unit tests  <span style="background-color: #FFFF00; color: #000000; padding: 5px; border-radius: 10px;">In Development</span> 
- [ ] GP loop <span style="background-color: #87CEEB; color: #000000; padding: 5px; border-radius: 10px;">Started Development</span>
- [ ] documentation 


Future work:
- [ ] GP loop
  - [ ] GP loop tests
    - [ ] unit tests
    - [ ] integration tests
    - [ ] performance tests
  - [ ] Look into parallelization of the GP loop
  - [ ] deploying in hpcc??
  - [ ] implementation for scavenger queue?

- [ ] Training on a sample world.
  
Current Plan:
- check the validity of gp loop by testing it on a public gp project
- run LGP agent on a sample maze world and make markers for Euclidean distance traveled and time taken/steps taken.
- (unsure) run each agent from different starting points to ensure that there is no starting bias??
- (unsure) potentially start a blind run after initial approval from instructors about LGPA and CGPA.

Question for others:
- Structure for actions: are we going to have access to all the actions all of the time or will it just be a subset?
- Expected behavior for agents: what are some exceptions for the automated agents?
- Integration Testing: How to test CGP and LGP agents?
- Integration Testing: How to test GP loop?
- Testing: How to test on training data?
- Refactor: Move Sensors up to core?? as it is agnostic to the agent type.
  - is it useful to any other group??