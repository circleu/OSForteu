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
db 1 ; type | 0x00 file | 0x01 directory
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
db 0
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
db 0
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

times (128+8+(32 * 99))*512 - ($ - $$) db 0
; 100th file
db 'far'
times 10 db 0
db 0
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
db 0
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

times (128+8+65536+65536+(8*1))*512 - ($ - $$) db 0

db 'Hello'

times (128+8+65536+65536+(8*2))*512 - ($ - $$) db 0

db 'World'

times (128+8+65536+65536+(8*3))*512 - ($ - $$) db 0

db 'This file is in the root directory.'

times (128+8+65536+65536+(8*4))*512 - ($ - $$) db 0

db 'This file is located at offset 4.'

times (128+8+65536+65536+(8*5))*512 - ($ - $$) db 0