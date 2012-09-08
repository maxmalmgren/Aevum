using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Machine.Specifications;

namespace Max.Aevum.Test
{
    public class When_querying_for_resolution
    {
        private static Resolution res;

        Because of = () => res = HighResolutionTimer.GetResolution();

        It should_return_a_minimum_resolution_greater_than_0 = () => res.GetMinimumResolution().ShouldBeGreaterThan(0u);
    }
}
