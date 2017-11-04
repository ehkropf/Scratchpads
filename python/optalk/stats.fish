#!/bin/fish
#

set msgs message-2.8.8.log message-2.11.1.log
set reply reply-2.8.8.log reply-2.11.1.log

for f in $msgs
    echo
    echo Messages in $f:
    set lines (grep -o -E 'message_type=[a-zA-Z_]+' $f | sort | uniq)
    for i in $lines
        echo -n "$i: "
        echo (grep $i $f | wc -l)
    end
end

for f in $reply
    echo
    echo Replies in $f:
    set lines (grep -o -E -o -E 'reply_to=+\{message_type=[a-zA-Z_]+' $f | sort | uniq)
    for i in $lines
        echo -n "$i: "
        echo (grep $i $f | wc -l)
    end
end
