times 128*512 db 0

dd (128+8) ; file table start sector
dd 65536 ; file table size (sectors)
db 32 ; file size (bytes)

dd (128+8+65536) ; dptr table start sector
dd 65536 ; dptr table size (sectors)
db 32 ; dptr size (bytes)

dd (128+8+65536+65536) ; data area start sector

times (128+8)*512 - ($ - $$) db 0

times (128+8+65536)*512 - ($ - $$) db 0

times (128+8+65536+65536)*512 - ($ - $$) db 0