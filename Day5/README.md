Day 5 - Binary Seat Allocation
----

This challenge involves taking seat addresses in form:


```FFBBBBBLLR```

and converting them into a row and seat number.

The address above is already binary, just mapped from (F)ront, (B)ack, (L)eft and (R)ight, so the task is quite simple:

 - Map chars to 0,1
 - Split into two binary numbers length 7 and 3
 - std::stoi to convert these binary strings to ints


 