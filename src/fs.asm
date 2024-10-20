times 128*512 db 0

dd (128+8) ; file table start sector
dd 65536 ; file table size (sectors)
db 32 ; file size (bytes)

dd (128+8+65536) ; ptr table start sector
dd 65536 ; ptr table size (sectors)
db 32 ; ptr size (bytes)

dd (128+8+65536+65536) ; data area start sector

times (128+8)*512 - ($ - $$) db 0

db 'root' ; name
times 9 db 0
db 0x11 ; type | 0x~0 file | 0x~1 directory
        ;      | 0x0~ read/write | 0x1~ read-only | 0x2~ inaccessible
dd 0 ; ptr table offset
; create date
dw 1970 ; year
db 1 ; month
db 1 ; date
db 0 ; hour
db 0 ; minute
db 0 ; second
; edit date
dw 1970 ; year
db 1 ; month
db 1 ; date
db 0 ; hour
db 0 ; minute
db 0 ; second

db 'foo'
times 10 db 0
db 0x00
dd 1
; create date
dw 1970
db 1
db 1
db 0
db 0
db 0
; edit date
dw 1970
db 1
db 1
db 0
db 0
db 0

db 'bar'
times 10 db 0
db 0x00
dd 2
; create date
dw 1970
db 1
db 1
db 0
db 0
db 0
; edit date
dw 1970
db 1
db 1
db 0
db 0
db 0

times (128+8)*512 + (32 * 4095) - ($ - $$) db 0
; 4096th file
db 'far'
times 10 db 0
db 0x00
dd 4
; create date
dw 1970
db 1
db 1
db 0
db 0
db 0
; edit date
dw 1970
db 1
db 1
db 0
db 0
db 0

times (128+8+65536)*512 - ($ - $$) db 0

db 1 ; status0 | 0x00 free | 0x01 one used | 0x02 two used | 0x03 three used | 0x04 four used | ...
db 0 ; status1 | 0x00 no pptr | 0x01 one pptr | 0x02 two pptr | 0x03 three pptr | 0x04 four pptr | ...
dd 0, 0, 0, 0, 0, 0, 0 ; data area offset | pptr starts from end
db 0, 0 ; reserved

db 1
db 0
dd 1, 0, 0, 0, 0, 0, 0
db 0, 0

db 1
db 0
dd 2, 0, 0, 0, 0, 0, 0
db 0, 0

db 1
db 0
dd 3, 0, 0, 0, 0, 0, 0
db 0, 0

db 1
db 0
dd 4, 0, 0, 0, 0, 0, 0
db 0, 0

times (128+8+65536+65536)*512 - ($ - $$) db 0

db 'inroot'
times 7 db 0
db 0x00
dd 3
; create date
dw 1970
db 1
db 1
db 0
db 0
db 0
; edit date
dw 1970
db 1
db 1
db 0
db 0
db 0

db 'root_hw'
times 6 db 0
db 0x00
dd 5
; create date
dw 1970
db 1
db 1
db 0
db 0
db 0
; edit date
dw 1970
db 1
db 1
db 0
db 0
db 0

times (128+8+65536+65536+(8*1))*512 - ($ - $$) db 0

db 'Hello'

times (128+8+65536+65536+(8*2))*512 - ($ - $$) db 0

db 'World'

times (128+8+65536+65536+(8*3))*512 - ($ - $$) db 0

db 'This file is in the root directory.'

times (128+8+65536+65536+(8*4))*512 - ($ - $$) db 0

db 'This file is located at offset 4.'

times (128+8+65536+65536+(8*5))*512 - ($ - $$) db 0

db 'Hello, World! - from root directory.'

times (128+8+65536+65536+(8*6))*512 - ($ - $$) db 0