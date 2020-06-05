module dimacs
open System.IO
open System.Text.RegularExpressions

(* Write information about the source and the sink to file
 * Args:
     * s: The stream writer
     * V: n, number of vertices
     * E: m, number of edges *)
let writeSrcSink (s : StreamWriter) (V : int) (E : int) : unit =
    s.WriteLine (sprintf "p max %d %d" V E)
    s.WriteLine "n 1 s"
    s.WriteLine (sprintf "n %d t" V)

(* Write information about all edges to file
 * Args:
     * s:     The stream writer
     * graph: The graph
     * V:     n, number of vertices *)
let writeEdges (s : StreamWriter) (graph : int [,]) (V : int) : unit =
    for u = 0 to V - 2 do
        for v = 1 to V - 1 do
            let weight = graph.[u,v]
            // If edge exist, it has a weight, then write it to the file
            if weight > 0 then
                s.WriteLine (sprintf "a %d %d %d" (u+1) (v+1) weight)
            else ()

(* Make new file and write all information about graph to it
 * Args:
     * graph: The graph
     * E:     m, the number of edges
     * bound: 'max' or 'min'
     * c:     The hidden constant *)
let DIMACS (graph : int [,]) (E : int) (bound : string) (c : string) : unit =
    let V = Array2D.length1 graph
    // build file name
    let mutable now = Regex.Replace(string System.DateTime.Now, @"\s+", "_")
    let name = sprintf "graphs/V%s-%s-c%s_E%s_%s" (string V) bound c (string E)
                                      <| Regex.Replace(now, @"/", "-")
    //printfn "%A" name
    let stream : StreamWriter = File.CreateText name

    writeSrcSink stream V E
    writeEdges stream graph V

    stream.Close()

