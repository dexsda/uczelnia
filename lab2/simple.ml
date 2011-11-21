let rec int2bin n = match n with
        | 0 -> "0"
        | 1 -> "1"
        | _ -> int2bin(n/2) ^ int2bin (n mod 2)

let rec power x y = if y = 1 then
        x else if y mod 2 = 0 then
        power x (y/2)*power x (y/2) else 
        x * power x (y/2)*power x (y/2)

let rec power2 x y = 
        if y = 1 then
        x else if y mod 2 = 0 then
        let pow = power2 x (y/2) in pow * pow else
        let pow = power2 x (y/2) in pow * pow * x
