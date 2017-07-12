import java.io.File

var FILENAME = System.getenv("HOME") + "/.todolist"
var COLOR_NEUTRAL = '\u001B' + "[0;0m"
var COLOR_TODO = '\u001B' + "[31m"
var COLOR_DONE = '\u001B' + "[32m"

class Task {
    var name: String
    var color: String
    var status: Int
    
    constructor(taskStr: String) {
        var task = taskStr.split(";")
        this.name = task[0]
        this.status = if (task.size > 1) Integer.valueOf(task[1]) else 0
        this.color = if (this.status == 0) COLOR_TODO else COLOR_DONE
    }

    override public fun toString() : String {
        return String.format("%s%s%s", this.color, this.name, COLOR_NEUTRAL)
    }

    public fun export() : String {
        return String.format("%s;%d\n", this.name, this.status)
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
    for (mode in modes.keys) {
        for (arg in args) {
            if (params[mode] == false || params[mode] == null) {
                params[mode] = (modes[mode]!!.contains(arg))
            }
        }
    }
    return params
}

fun usage() {
    println(String.format("usage : %s [add|rm|status] task", "todo"))
}

fun getTasks(): MutableList<Task> {
    var file = File(FILENAME)
    var lines = file.readLines()
    var tasks = mutableListOf<Task>();
    for (line in lines) {
        tasks.add(Task(line))
    }
    return tasks
}

fun saveTasks(tasks: MutableList<Task>) {
    var file = File(FILENAME)
    var output = ""
    for (task in tasks) {
        output += task.export()
    }
    file.writeText(output)
}

fun main(args: Array<String>) {
    var params = handleArgs(args)
    if (params["help"] == true) {
        return usage()
    }
    var tasks = getTasks()
    
    if (args.size == 2) {
        if (params["add"] == true) {
            tasks.add(Task(args[1]))
        }
        else {
            var id = Integer.valueOf(args[1])
            if (id >= 0 && id < tasks.size) {
                if (params["status"] == true) {
                    tasks[id].status = if (tasks[id].status == 1) 0 else 1
                }
                else if (params["rm"] == true) {
                    tasks.removeAt(id)
                }
            }
        }
        saveTasks(tasks)
    }    
    for (task in tasks) {
        println(task)
    }
}
