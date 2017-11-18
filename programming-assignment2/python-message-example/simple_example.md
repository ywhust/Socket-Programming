
# How to "Encode" Message with Arbitrary Format

The following gives you two solutions, of course they are not the only solutions. You could come up with your own solution if you would like to.


## Solution 1: Using Struct

For example, you are asked to encode a socket message in the following format: `msg length (1 byte) msg`, which contains two parts: The first one is `msg length`, and the second one is `msg`, whose length is given by `msg length`. You use [python struct](https://docs.python.org/2/library/struct.html) to achieve this goal as follows. 


```python
import struct

msg = 'I am a simple example!!'
msg_length = len(msg)
print "msg_length = ", msg_length
# here, 'B' is for unsigned char, s stands for char[], the number before it gives its length
raw = struct.pack('B{}s'.format(msg_length), msg_length, msg)
print "row = ", raw
print "len(raw) = ", len(raw)
```

    msg_length =  23
    row =  I am a simple example!!
    len(raw) =  24


Now you can send `raw` via socket. After the other side receives the `raw`, it can use unpack to decode the original message. Note that, it seems struct does not provide `uint8`, so in the example, I used `unsigned char`. But as explained in [Format Characters](https://docs.python.org/2/library/struct.html#format-characters), it will be converted to a `Python Integer` automatically.


```python
msg_length = struct.unpack('B', raw[0])[0]
print type(msg_length)
print msg_length
msg = struct.unpack('{}s'.format(msg_length), raw[1:])[0]
print msg
```

    <type 'int'>
    23
    I am a simple example!!


## Solution 2: Using `ascii`

Please see the following example.


```python
new_msg = "I am another example!!"
new_msg_length = chr(len(new_msg))
print 'new_message_length = ', new_msg_length
raw = new_msg_length + new_msg
print 'raw = ', raw
```

    new_message_length =  
    raw =  I am another example!!



```python
# Now, decode it.
msg_length = ord(raw[0])
# msg = raw[1:msg_length] here i made a mistake in previous version
msg = raw[1:(msg_length + 1)]
print "msg_length = ", msg_length
print "msg = ", msg
```

    msg_length =  22
    msg =  I am another example!!

