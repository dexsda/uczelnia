let reverseInt integer = 
        let rec reverseAccum integer accum = match integer with
                | 0 -> accum
                | i -> reverseAccum (i/10) (accum*10 + i mod 10) in
        reverseAccum integer 0
