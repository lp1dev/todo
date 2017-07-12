import java.io.File

class Task{
    var id: Int
    var name: String
    var status: Int
    
    constructor(id: Int, task: List<String>) {
        this.id = id
        this.name = task[0]
        this.status = Integer.valueOf(task[1])
    }

    override public fun toString() : String{
        return String.format("[Task %s] %s", this.id, this.name)
    }
}

fun handleArgs(args: Array<String>): MutableMap<String, Boolean> {
    var params = mutableMapOf<String, Boolean>()
    var modes = mapOf(
            "help" to arrayOf("-h", "--help"),
            "add" to arrayOf("add"),
            "rm" to arrayOf("rm"),
            "status" to arrayOf("status")
    )
    for (arg in args) {
        for (mode in modes.keys) {
            params[mode] = (modes[mode]!!.contains(arg))
        }
    }
    return params
}

fun usage() {
    println(String.format("usage : %s [add|rm|status] task", "todo"))
}

fun getTasks(): MutableList<Task> {
    var file = File("/home/lp1/.todolist")
    var lines = file.readLines()
    var tasks = mutableListOf<Task>();
    for (line in lines) {
        tasks.add(Task(tasks.size, line.split(";")))
    }
    return tasks
}

fun main(args: Array<String>) {
    var params = handleArgs(args)
    if (params.keys.count() == 0 || params["help"] == true) {
        return usage()
    }
    var tasks = getTasks()
    for (task in tasks) {
        println(task)
    }
}
