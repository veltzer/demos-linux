Message_Blocks     - Create a message block.
                     Store data in the block.
                     Move the write pointer and concatenate additional string.
prioriy_buffer.cpp - Read lines from STDIN insert into queue according to priority
                     Read from the head buffer. - DataFile as input.
head_buffer.cpp    - Read lines from STDIN insert into queue at the tail
                     Read from the head buffer. - DataFile as input.
tail_buffer.cpp    - Read lines from STDIN insert into queue at the tail
                     Read from the tail buffer. - DataFile as input.
                     Nothing is printerd since the termination signal is zero length
                     and the last message (first that is read in the tail) is zero length.
Timer.cpp            Create 10 timers, cancel #5 and see what happens.
Get_Opt.cpp          Demostrate GetOpt:
                     options are: -f            file FileName
                                  -g
                                  -h
                                  -l parameter
                                  --cool_option               - we get 0 into the switch
                                  --the_answer                - we get 42 into the switch
