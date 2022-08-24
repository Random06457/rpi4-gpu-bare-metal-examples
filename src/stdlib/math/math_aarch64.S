.global floor
floor:
    frintm d0, d0
    ret

.global ceil
ceil:
    frintp d0, d0
    ret