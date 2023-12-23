import Data.List
import Data.Foldable
import Data.Function
import System.CPUTime
import Text.Printf

data Task = Task { taskId :: Int, taskTime :: Int }
data Machine = Machine { totalTime :: Int, assignedTasks :: [Int] }

consWhen :: Bool -> a -> [a] -> [a]
consWhen p a = if p then (a :) else id

newMachine :: Task -> Machine
newMachine (Task i t) = Machine t [i]

updateMachine :: Task -> Machine -> Machine
updateMachine (Task i t) (Machine t' i') = Machine (t + t') (i : i')


assignTask :: Int -> Int -> Task -> [Machine] -> [[Machine]]
assignTask k limit task machines =
  consWhen (length machines < k) (newMachine task : machines) (f machines)
  where
    f [] = []
    f (m : ms) = consWhen (totalTime m + taskTime task <= limit)
                    (updateMachine task m : ms) [m : l | l <- f ms]


strategies :: [Task] -> Int -> Int -> [[Machine]]
strategies tasks k limit = f tasks []
  where
    f [] machines = [machines]
    f (t : ts) machines = concatMap (f ts) (assignTask k limit t machines)


searchMinTime :: [Task] -> Int -> Int -> Int -> [[Machine]]
searchMinTime tasks k l r =
  case compare l r of
    GT -> undefined
    EQ -> strategies tasks k r
    LT -> let mid = (l + r) `div` 2 in
          if null (strategies tasks k mid)
            then searchMinTime tasks k (mid + 1) r
            else searchMinTime tasks k l mid


bestStrategies :: [Task] -> Int -> [[Machine]]
bestStrategies tasks k = searchMinTime sorted k l r
  where
    sorted = sortBy (flip compare `on` taskTime) tasks
    times = map taskTime tasks
    l = sum times `div` k
    r = sum times


runTest :: Int -> IO ()
runTest c = do
  printf "Running test %d\n" c
  _ : k : times <- map read . words <$> readFile (printf "test%d.txt" c)
  let tasks = zipWith Task [1..] times
  let machines = head $ bestStrategies tasks k
  start <- getCPUTime
  machines `seq` return ()
  end <- getCPUTime
  let diff = fromIntegral (end - start) / (10^12)
  printf "Algorithm completed in %.6fs\n" (diff :: Double)
  printf "Solution:\n"
  printf "Min time required: %d\n" (maximum $ map totalTime machines)
  printf "A strategy:\n"
  forM_ (zip [1..] machines) $ \(i, Machine _ t) -> do
    printf "  Machine %d: tasks = %s\n" (i :: Int) (show $ sort t)
  printf "\n"


main :: IO ()
main = mapM_ runTest [1, 2, 3]
