queue = {}
queue.__index = queue

function queue.new()
    local q = { front = 1, back = 1 }
    return setmetatable(q, queue)
end

function queue.empty(q)
    return q.front == q.back
end

function queue.push(q, v)
    q[q.back] = v
    q.back = q.back + 1
end

function queue.pop(q)
    if q:empty() then
        error("queue is empty")
    end
    local v = q[q.front]
    q[q.front] = nil
    q.front = q.front + 1
    return v
end

function part1()
    local file = io.open("data.txt", "r")
    local n = 1
    local index = {}
    local data = {}
    for v in file:read():gmatch("%a") do
        index[v] = n
        data[n] = v
        n = n + 1
    end

    local matrix = {}
    for i = 1, n do
        matrix[i] = {}
        for j = 1, n do
            matrix[i][j] = false
        end
    end

    for from, to in file:read("a"):gmatch("(%a)-(%a)") do
        local i, j = index[from], index[to]
        matrix[i][j] = true
        matrix[j][i] = true
    end
    file:close()

    local discovered = { [1] = true }
    local q = queue.new()
    q:push(1)
    local order = {}
    repeat
        local i = q:pop()
        table.insert(order, data[i])
        for j = 1, n do
            if matrix[i][j] and not discovered[j] then
                discovered[j] = true
                q:push(j)
            end
        end
    until q:empty()
    print(table.concat(order, " -> "))
end

function part2()
    print("Loading data...")
    local adj = {}
    for u, v in io.lines("twitter_small.txt", "n", "n") do
        adj[u] = adj[u] or {}
        adj[v] = adj[v] or {}
        table.insert(adj[u], v)
    end
    print("done.")

    local t = os.clock()
    local discovered = {}
    for k, _ in pairs(adj) do
        if not discovered[k] then
            discovered[k] = true
            local count = 0
            local q = queue.new()
            q:push(k)
            repeat
                local u = q:pop()
                count = count + 1
                for _, v in ipairs(adj[u]) do
                    if not discovered[v] then
                        discovered[v] = true
                        q:push(v)
                    end
                end
            until q:empty()
            print(("start = %d, count = %d"):format(k, count))
        end
    end
    print(("Traverse all nodes in %.3fs"):format(os.clock() - t))
end

print("==================== part 1 ====================")
part1()
print("==================== part 2 ====================")
part2()
