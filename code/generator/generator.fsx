open dimacs
(* The purpose of this program is to build a graph and write it in DIMACS
 * maximum flow format to a textfile in the directory 'graphs/'

 * Compile: fsharpc dimacs.fs generator.fsx
 * Execute: mono generator.exe <Edges' max capacity> <bound> <|V|> <c>
 * Look in README.md for more information
 *)

let rnd = System.Random()

(* Get a vertex to connect vertex 'u' to with an edge.
 * Args:
     * u:           Vertex that needs an edge
     * maxV:        The highest index a possible vertex can have
     * graph:       The graph
     * findOutEdge: If True (u,v) u->v is found, otherwise (v,u) v->u is found
 * Returns:
     * Vertex v to connect u to *)
let findVertex (u : int) (maxV : int) (graph : int [,]) (findOutEdge : bool) : int =
    let mutable v : int = u

    if findOutEdge then
        // From u to random vertex 1 to t
        // While a reverse edge or a selfloop exists find a new v
        while graph.[v, u] > 0 || v = u do
            v <- rnd.Next(1, maxV)
    else
        // Into u from random vertex 0 to t-1
        while graph.[u, v] > 0 || v = u do
            v <- rnd.Next(0, maxV - 1)
    v

(* Increment u and v's count of edges going into or out of them in the
 * array 'edgesCounter'.
     * edgesCounter: Array with counts of each vertex' edges
     * u: Vertex
     * v: Vertex *)
let incEdgesCounter (edgesCounter : int array) (u : int) (v : int) : unit =
    edgesCounter.[u] <- edgesCounter.[u] + 1
    edgesCounter.[v] <- edgesCounter.[v] + 1

(* Add edge between (u,v) with random capacity from 1 to max capacity to the
 * 2D array 'graph'.
 * Args:
     * graph:        The graph to add new edge to
     * maxCapacity:  Max allowed capacity on edges
     * u:            Vertex the edge emerges from
     * v:            Vertex the edge goes into *)
let addEdge (graph : int [,]) (maxCapacity : int) (u : int) (v : int) : unit =
    graph.[u,v]  <- rnd.Next(1, maxCapacity + 1)

(* Add edges going out of all vertices in V \ t
 * Args:
     * graph:        The graph to add edges to
     * edgesCounter: Array with counts of each vertex' edges
     * V:            Number of vertices
     * maxCapacity:  Max allowed capacity on edges
 * Returns:
     * The number of edges made *)
let addOutEdges (graph : int [,]) (edgesCounter : int []) (V : int)
                                                     (maxCapacity : int) : int =
    let mutable cnt : int = 0

    for u = 0 to V - 2 do
        let v : int = findVertex u V graph true
        addEdge graph maxCapacity u v
        incEdgesCounter edgesCounter u v
        cnt <- cnt + 1
    cnt

(* Add edges going into all vertices in V \ s. Makes sure to only add an edge
 * into  a vertex if its current number of edges is less than |V| - 1.
 * Args:
     * graph:        The graph to add edges to
     * edgesCounter: Array with counts of each vertex' edges
     * V:            Number of vertices
     * maxCapacity:  Max allowed capacity on edges
 * Returns:
     * The number of edges made *)
let addInEdges (graph : int [,]) (edgesCounter : int []) (V : int)
                                                     (maxCapacity : int) : int =
    let mutable cnt : int = 0

    for v = 1 to V - 1 do
        if edgesCounter.[v] < V - 1 then
            let u : int = findVertex v V graph false
            if graph.[u,v] = 0 then
                addEdge graph maxCapacity u v
                incEdgesCounter edgesCounter u v
                cnt <- cnt + 1

    if edgesCounter.[V - 1] < V - 1 then
        let u : int = findVertex (V - 1) V graph false
        if graph.[u, V - 1] = 0 then
            addEdge graph maxCapacity u (V - 1)
            incEdgesCounter edgesCounter u (V - 1)
            cnt <- cnt + 1
    cnt

(* Find all vertices connected to vertex u with edges going outwards from u.
 * Args:
     * graph: The graph
     * V:     Number of vertices
     * conn:  Array to set flags in when vertices are connected to u
     * u:     Vertex *)
let getConnectedVerticesOUT (graph : int [,]) (V : int) (conn: int array)
                                                              (u : int) : unit =
    let rec findEdges (v : int) : unit =
        for i = 1 to V - 1 do
            if graph.[v, i] > 0 && conn.[i] = 0 then
                conn.[i] <- 1
                findEdges i
            else ()
    findEdges u

(* Find all vertices connected to vertex u with edges going towards or into u.
 * Args:
     * graph: The graph
     * V:     Number of vertices
     * conn:  Array to be updated for telling which vertices are connected to u
     * u:     Vertex *)
let getConnectedVerticesIN (rel : int [,]) (V : int) (conn: int array) (u : int)
                                                                        : unit =
    let rec findEdges (ind : int) : unit =
        for i = 0 to V - 2 do
            if rel.[i, ind] > 0 && conn.[i] = 0 then
                conn.[i] <- 1
                if i > 0 then
                    findEdges i
                else ()
            else ()
    findEdges u

(* Find all vertices connected to the source and the sink
 * Args:
     * S:     Array to mark connections to the source
     * T:     Array to mark connections to the sink
     * graph: The graph
     * V:     Number of vertices *)
let findRelationWith_s_t (S : int array) (T : int array) (graph : int [,])
                                                              (V : int) : unit =
    S.[0] <- 1    // The source, s, is connected to itself
    T.[V-1] <- 1  // The sink, t, is connected to itself

    // Find vertices connected to the source, s
    getConnectedVerticesOUT graph V S 0
    // Find vertices connected to the sink, t
    getConnectedVerticesIN graph V T (V-1)


(* Add n random legal edges
 * Args:
     * n:            The amount of edges to be added
     * V:            Number of vertices in graph
     * maxCapacity:  Max allowed capacity on edges
     * graph:        The graph to add edges to
     * edgesCounter: Array with counts of each vertex' edges *)
let addNEdges (n : int) (V : int) (maxCapacity : int) (graph : int[,])
                                             (edgesCounter : int array) : unit =
    let mutable cnt : int = 0
    while cnt < n do
        let u : int = rnd.Next(0, V - 1)
        if edgesCounter.[u] < V - 1 then
            let v : int = findVertex u V graph true
            if graph.[u,v] = 0 then
                addEdge graph maxCapacity u v
                incEdgesCounter edgesCounter u v
                cnt <- cnt + 1

(* Checking if graph is fully connected and every vertex is connected to the
 * source and the sink
 * Args:
     * V:     Number of vertices in graph
     * graph: The graph
 * Returns:
     * True if the graph is correct otherwise false *)
let isGraphConnected (V : int) (graph : int[,]) : bool =
        let S : int array = Array.zeroCreate V
        let T : int array = Array.zeroCreate V
        findRelationWith_s_t S T graph V
        Array.sum S = V && Array.sum T = V

(* Builds a graph with |V| vertices and 'edgesBound' edges
 * Args:
     * V:           Number of vertices in graph
     * maxCapacity: Max allowed capacity on edges
     * edgesBound:  The number of edges to make
 * Returns:
     * An option type.
        * 'None' if a graph is not build
        *  otherwise a 'Some (graph, number of edges)' *)
let buildRandomGraph (V : int) (maxCapacity : int) (edgesBound : int)
                                                    : (int [,] * int) option =
    let graph : int [,]   = Array2D.create V V 0
    let edgesCounter : int array = Array.zeroCreate V
    let minEdges : int = V - 1
    let maxEdges : int = V * (V - 1) / 2

    // Add edges going out from and into all vertices
    let mutable E : int =
        addOutEdges graph edgesCounter V maxCapacity
        +
        addInEdges graph edgesCounter V maxCapacity

    // Find remaning number of edges to be made
    let addAmount = if E < edgesBound then
                        edgesBound - E
                    else if E = edgesBound then
                          0
                    else -1

    if addAmount <= -1 then
        None
    else
        addNEdges addAmount V maxCapacity graph edgesCounter
//  printfn "\n|V|:\t\t%d\n|E|:\t\t%d\nMin. |E|:\t%d\nMax. |E|:\t%d\n
// max capacity: %d\n" V E minEdges maxEdges maxCapacity

        E <- E + addAmount

        if isGraphConnected V graph then
            Some (graph, E)
        else
            None

(* Checking for antiparallel edges
 * Args:
     * graph: The graph
 * Returns:
     * true if there is no antiparallel edges otherwise false *)
let checkReverseEdges (graph: int [,]) : bool =
    let mutable ans : bool = true
    for i = 0 to Array2D.length1 graph - 1 do
        for j = 0 to Array2D.length2 graph - 1 do
            if i <> j  then
                if graph.[i,j] > 0 && graph.[j,i] > 0 then
                    ans <- false
                else
                    ans <- ans
    ans

(* The main function which takes command line arguments
 * Args:
    * arg 0 : Edges' max capacity
    * arg 1 : max or min. Telling if I am setting an upper bound or lower bound
    *          for number of edges
    * arg 2 : Number of vertices
    * arg 3 : The hidden constant to scale with
 * Returns:
     * 1 on failure - a graph was not build
     * 0 on success - a graph was build and written to a text file *)
[<EntryPoint>]
let main args =
    if args.Length <> 4 then
        raise (System.ArgumentException
                 "Error: Wrong number of arguments. Expects four.")
        1
    else
        let maxCapacity = int args.[0]
        let n = float args.[2]
        let c = float args.[3]

        let edgesBound =
            match args.[1] with
            | "max" -> int <| System.Math.Floor (c * n) // Looking at Theta(n)
            | "min" -> int <| System.Math.Floor (c * pown n 2) // Looking at Theta(n^2)
            | _ -> raise (System.ArgumentException
                             "Error: Arg 1 must be 'max' or 'min'")

        match buildRandomGraph (int n) maxCapacity edgesBound with
        | Some(graph, E) ->
                  //printfn "\n%A." <| checkReverseEdges graph
                  if checkReverseEdges graph then
                      DIMACS graph E args.[1] args.[3]
                      0
                  else
                      1
        | None -> 1

