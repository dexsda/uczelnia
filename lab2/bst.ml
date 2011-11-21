type 'a btree =
        | Nil
        | Node of 'a *  'a btree * 'a btree

let rec insert t x = match t with
        | Nil -> Node (x, Nil, Nil)
        | Node (y, left, right) ->
                if x <= y then Node (y, insert left x, right)
                else Node (y, left, insert right x)

let buildTree1 () = Node(5, Node(3, Node(1, Nil, Nil), Node(4, Nil, Nil)), Node(8, Nil, Nil))

let buildTree2 () = insert(insert(insert(insert(insert Nil 5) 3) 1) 4) 8

let rec member node value = match node with
        | Node (a, l, r) -> 
                if a == value then true
                else if value < a then member l value
                else member r value
        | Nil -> false

let rec keys node = match node with
        | Nil -> []
        | Node (a, l, r) -> keys l @ [a] @ keys r

let rec height node = match node with
        | Nil -> -1
        | Node (a, l, r) -> let max a b = if a>b then a else b in
                1+max (height l) (height r)

let rec size node = match node with
        | Nil -> 0
        | Node (a, l, r) -> 1 + size l + size r

