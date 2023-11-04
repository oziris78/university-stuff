package oguzhantopaloglu;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Std {

    public static class ScoreMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        private final IntWritable score = new IntWritable();
        private final Text keyText = new Text("score");

        @Override
        public void map(LongWritable key, Text value, Context context) {
            String line = value.toString();
            String[] columns = line.split(",");

            if (columns.length == 6) {
                try {
                    score.set(Integer.parseInt(columns[3]));
                    context.write(keyText, score);
                }
                catch (Exception e) {}
            }
        }
    }

    public static class ScoreReducer extends Reducer<Text, IntWritable, Text, DoubleWritable> {
        private final DoubleWritable standardDeviation = new DoubleWritable();

        @Override
        public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
            List<Integer> scoreList = new ArrayList<>();
            double sum = 0.0;
            int count = 0;

            for (IntWritable value : values) {
                int score = value.get();
                scoreList.add(score);
                sum += score;
                count++;
            }

            double mean = sum / count;

            double sumOfSquaredDiff = 0.0;
            for (int score : scoreList) {
                double diff = score - mean;
                sumOfSquaredDiff += diff * diff;
            }

            double variance = sumOfSquaredDiff / count;
            double standardDev = Math.sqrt(variance);

            standardDeviation.set(standardDev);
            context.write(key, standardDeviation);
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Standard Deviation");

        job.setJarByClass(Std.class);
        job.setMapperClass(ScoreMapper.class);
        job.setReducerClass(ScoreReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        /*
        long starting, ending;
        starting = System.currentTimeMillis();
        job.waitForCompletion(true);
        ending = System.currentTimeMillis();
        System.out.println("Elapsed time: " + (ending-starting)); // or show it in GUI.
        */
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}
