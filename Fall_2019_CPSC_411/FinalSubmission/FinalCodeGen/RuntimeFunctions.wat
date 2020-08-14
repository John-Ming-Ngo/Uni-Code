(module	 
  (import "host" "exit" (func $halt))
  (import "host" "getchar" (func $getchar (result i32)))
  (import "host" "putchar" (func $printc (param i32)))
  
  (memory $strs 1)
  
  (func $printb 
	(param $bool i32)
	(block $falsePrint
		i32.const 1
		local.get $bool
		i32.eq
		br_if $falsePrint
		i32.const 4
		i32.const 9
		call $prints
	)
	(block $truePrint
		i32.const 0
		local.get $bool
		i32.eq
		br_if $truePrint
		i32.const 0
		i32.const 4
		call $prints
	)
	
  )
  
  (func $printNum (param $printNumint i32)
	(block $end
	(block $9
	(block $8
	(block $7
	(block $6
	(block $5
	(block $4
	(block $3
	(block $2
	(block $1
	(block $NUM0
	local.get $printNumint
	i32.const 0
	i32.eq
	br_if $NUM0
	local.get $printNumint
	i32.const 1
	i32.eq
	br_if $1
	local.get $printNumint
	i32.const 2
	i32.eq
	br_if $2
	local.get $printNumint
	i32.const 3
	i32.eq
	br_if $3
	local.get $printNumint
	i32.const 4
	i32.eq
	br_if $4
	local.get $printNumint
	i32.const 5
	i32.eq
	br_if $5
	local.get $printNumint
	i32.const 6
	i32.eq
	br_if $6
	local.get $printNumint
	i32.const 7
	i32.eq
	br_if $7
	local.get $printNumint
	i32.const 8
	i32.eq
	br_if $8
	local.get $printNumint
	i32.const 9
	i32.eq
	br_if $9
	)
	i32.const 48
	call $printc
	br $end
	)
	i32.const 49
	call $printc
	br $end
	)
	i32.const 50
	call $printc
	br $end
	)
	i32.const 51
	call $printc
	br $end
	)
	i32.const 52
	call $printc
	br $end
	)
	i32.const 53
	call $printc
	br $end
	)
	i32.const 54
	call $printc
	br $end
	)
	i32.const 55
	call $printc
	br $end
	)
	i32.const 56
	call $printc
	br $end
	)
	i32.const 57
	call $printc
	br $end
	)
	
  )
  
  (func $printi2
	(param $intVal i32)
	(local $localIntVal i32)
	local.get $intVal
	local.set $localIntVal
	(block $printiB1
		(loop $printiL1
			local.get $localIntVal
			i32.const 0
			i32.eq
			br_if $printiB1
			
			local.get $localIntVal
			i32.const 10
			i32.div_s
			call $printi2
			
			local.get $localIntVal
			i32.const 10
			i32.rem_s
			call $printNum
		)
	)
  )
  
  (func $printi ;;TODO: One problem ATM: zero. Ok made a butchery of a solution.
	(param $intVal i32)
	(local $localIntVal i32)
	local.get $intVal
	local.set $localIntVal
	(block $printNumZero
		local.get $localIntVal
		i32.const 0
		i32.ne
		br_if $printNumZero
		
		local.get $localIntVal
		call $printNum
	)
	(block $printiB1
		(loop $printiL1
			local.get $localIntVal
			i32.const 0
			i32.eq
			br_if $printiB1
			
			local.get $localIntVal
			i32.const 10
			i32.div_s
			call $printi2
			
			local.get $localIntVal
			i32.const 10
			i32.rem_s
			call $printNum
		)
	)
  )
  
  
  (func $prints 
	  (param $strStart i32) 
	  (param $strEnd i32)
	  (local $strCounter i32)
	  local.get $strStart
	  local.set $strCounter
	  (block $printsB1
		(loop $printsL1
		
		local.get $strCounter
		local.get $strEnd
		i32.ge_s
		br_if $printsB1
		
		local.get $strCounter 
		i32.load8_u
		call $printc
		
		local.get $strCounter
		i32.const 1
		i32.add
		local.set $strCounter
		
		br $printsL1
		)
	  )
  )
  
  (func $main
	i32.const 0
	call $printi
  )
  (start $main)
)

